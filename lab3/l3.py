import numpy as np
import matplotlib.pyplot as plt

MOD = (1 << 31) - 1
MULT = 16807

seed_val = 100


def uniform_gen(x):
    x = (MULT * x) % MOD
    value = x / MOD
    return x, value


P1 = np.array([
    [0.5, 0.3, 0.1, 0.1],
    [0.2, 0.4, 0.2, 0.2],
    [0.1, 0.2, 0.5, 0.2],
    [0.2, 0.1, 0.3, 0.4]
])

P2 = np.array([
    [0.4, 0.3, 0.1, 0.2],
    [0.1, 0.5, 0.2, 0.2],
    [0.2, 0.1, 0.4, 0.3],
    [0.0, 0.0, 0.0, 1.0]
])

P3 = np.array([
    [0.6, 0.4, 0.0, 0.0],
    [0.3, 0.7, 0.0, 0.0],
    [0.0, 0.0, 0.5, 0.5],
    [0.0, 0.0, 0.2, 0.8]
])

P4 = np.array([
    [0.4, 0.2, 0.3, 0.1],
    [0.1, 0.3, 0.4, 0.2],
    [0.0, 0.0, 0.7, 0.3],
    [0.0, 0.0, 0.4, 0.6]
])


M = 50
N = 50000


def get_next(P, state, rand):
    total = 0.0

    for nxt in range(len(P)):
        total += P[state][nxt]

        if rand < total:
            return nxt


def run_chain(P, start):
    global seed_val

    paths = []

    for _ in range(M):
        chain = [0] * (N + 1)
        chain[0] = start

        for i in range(N):
            seed_val, rand = uniform_gen(seed_val)
            chain[i + 1] = get_next(P, chain[i], rand)

        paths.append(chain)

    return paths


def estimate_distribution(paths):
    estimates = []

    for chain in paths:
        counts = [0, 0, 0, 0]

        for state in chain[1:]:
            counts[state] += 1

        estimates.append([c / N for c in counts])

    return np.array(estimates)


def inspect_chain(P, label):

    paths_0 = run_chain(P, 0)
    paths_3 = run_chain(P, 3)

    estimate_0 = estimate_distribution(paths_0)
    estimate_3 = estimate_distribution(paths_3)

    mat = P.T - np.eye(4)
    mat[3] = np.ones(4)

    rhs = np.array([0, 0, 0, 1])

    stationary = np.linalg.solve(mat, rhs)

    print("\n======================================")
    print(label)
    print("======================================")

    print("\nAnalytical stationary distribution:")
    print("pi =", np.round(stationary, 4))

    print("\nSimulated distribution for X0 = 0:")
    print("pi_hat =", np.round(estimate_0[0], 4))

    print("\nSimulated distribution for X0 = 3:")
    print("pi_hat =", np.round(estimate_3[0], 4))

    return stationary, estimate_0, estimate_3


inspect_chain(P1, "P1")
inspect_chain(P2, "P2")
inspect_chain(P3, "P3")
inspect_chain(P4, "P4")


def show_power(P, label):

    p100 = np.linalg.matrix_power(P, 100)

    print("\n======================================")
    print(label, "^100")
    print("======================================")
    print(np.round(p100, 4))


show_power(P1, "P1")
show_power(P2, "P2")
show_power(P3, "P3")
show_power(P4, "P4")


def running_proportion(chain):

    c0 = 0
    c1 = 0

    prob0 = []
    prob1 = []

    for i in range(1, N + 1):

        if chain[i] == 0:
            c0 += 1
        elif chain[i] == 1:
            c1 += 1

        prob0.append(c0 / i)
        prob1.append(c1 / i)

    return np.array(prob0), np.array(prob1)


paths = run_chain(P4, 0)

prob0, prob1 = running_proportion(paths[0])

steps = np.arange(1, N + 1)

print("pi_0(10)    =", prob0[9])
print("pi_0(100)   =", prob0[99])
print("pi_0(1000)  =", prob0[999])
print("pi_0(50000) =", prob0[49999])

print("pi_1(10)    =", prob1[9])
print("pi_1(100)   =", prob1[99])
print("pi_1(1000)  =", prob1[999])
print("pi_1(50000) =", prob1[49999])

plt.plot(steps, prob0, label="pi_hat_0(n)")
plt.plot(steps, prob1, label="pi_hat_1(n)")

plt.xlabel("n")
plt.ylabel("Empirical proportion")
plt.title("Case IV: Transient-State Proportions")
plt.legend()
plt.grid()
plt.show()
