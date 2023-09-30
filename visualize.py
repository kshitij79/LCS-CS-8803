import pandas as pd
import matplotlib.pyplot as plt
import numpy as np

# Read data from CSV
df = pd.read_csv('results_N150.csv')

# Extract relevant data
ratios = df['Ratio'].unique()
procedures = df['Procedure'].unique()

# Setup bar width and positions
barWidth = 0.25
r1 = np.arange(len(ratios))
r2 = [x + barWidth for x in r1]
r3 = [x + barWidth for x in r2]

# Timeout Threshold
TIMEOUT_THRESHOLD = 1000

# Function to handle bars with timeout
def handle_timeout(values):
    return [2.5 if x > TIMEOUT_THRESHOLD else x for x in values]


# Create bars for each procedure
# plt.bar(r1, df[df['Procedure'] == 'Benchmark Heuristic']['MedianTime'].values, color='g', width=barWidth, edgecolor='grey', label='Benchmark Heuristic')
# plt.bar(r2, df[df['Procedure'] == 'Two-Clause Heuristic']['MedianTime'].values, color='b', width=barWidth, edgecolor='grey', label='Two-Clause Heuristic')
# plt.bar(r3, df[df['Procedure'] == 'Random Heuristic']['MedianTime'].values, color='r', width=barWidth, edgecolor='grey', label='Random Heuristic', hatch='///' if df[df['Procedure'] == 'Random Heuristic']['MedianTime'].values >= TIMEOUT_THRESHOLD else '')
def get_hatch_patterns(values):
    return ['///' if x >= TIMEOUT_THRESHOLD else '' for x in values]

hatch_patterns_benchmark = get_hatch_patterns(df[df['Procedure'] == 'Benchmark Heuristic']['MedianTime'].values)
bars_benchmark = handle_timeout(df[df['Procedure'] == 'Benchmark Heuristic']['MedianTime'].values)

# Now when plotting, we assign the appropriate hatch pattern for each bar
for i, v in enumerate(bars_benchmark):
    plt.bar(r1[i], v, color='g', width=barWidth, edgecolor='grey', label='Random Heuristic' if i == 0 else "", hatch=hatch_patterns_benchmark[i])

hatch_patterns_twoClause = get_hatch_patterns(df[df['Procedure'] == 'Two-Clause Heuristic']['MedianTime'].values)
bars_twoCLause = handle_timeout(df[df['Procedure'] == 'Two-Clause Heuristic']['MedianTime'].values)

# Now when plotting, we assign the appropriate hatch pattern for each bar
for i, v in enumerate(bars_twoCLause):
    plt.bar(r2[i], v, color='b', width=barWidth, edgecolor='grey', label='Two-Clause Heuristic' if i == 0 else "", hatch=hatch_patterns_twoClause[i])


hatch_patterns_random = get_hatch_patterns(df[df['Procedure'] == 'Random Heuristic']['MedianTime'].values)
bars_random = handle_timeout(df[df['Procedure'] == 'Random Heuristic']['MedianTime'].values)

# Now when plotting, we assign the appropriate hatch pattern for each bar
for i, v in enumerate(bars_random):
    plt.bar(r3[i], v, color='r', width=barWidth, edgecolor='grey', label='Random Heuristic' if i == 0 else "", hatch=hatch_patterns_random[i])

# Title & Subtitle
plt.title('Compute Time vs L/N by Procedure (N=150)')
plt.xlabel('L/N Ratio', fontweight='bold')
plt.ylabel('Median Time (sec)')

# Add xticks on the middle of the grouped bars
plt.xticks([r + barWidth for r in range(len(ratios))], ratios)

# Create legend & Show graphic
plt.legend()
plt.grid(axis='y')
plt.tight_layout()
plt.savefig('compute_time_vs_L_N_grouped.png')
plt.clf()

# 
# 
# Plotting Number of DPLL calls vs L/N

# Function to handle bars with timeout
def handle_oversplit(values):
    return [2000 if x > TIMEOUT_THRESHOLD else x for x in values]

# Get hatch patterns for DPLL splits
hatch_patterns_splits_random = get_hatch_patterns(df[df['Procedure'] == 'Random Heuristic']['MedianSplits'].values)
bars_splits_random = handle_oversplit(df[df['Procedure'] == 'Random Heuristic']['MedianSplits'].values)

hatch_patterns_splits_benchmark = get_hatch_patterns(df[df['Procedure'] == 'Benchmark Heuristic']['MedianSplits'].values)
bars_splits_benchmark = handle_oversplit(df[df['Procedure'] == 'Benchmark Heuristic']['MedianSplits'].values)

hatch_patterns_splits_twoClause = get_hatch_patterns(df[df['Procedure'] == 'Two-Clause Heuristic']['MedianSplits'].values)
bars_splits_twoClause = handle_oversplit(df[df['Procedure'] == 'Two-Clause Heuristic']['MedianSplits'].values)

# plt.bar(r1, df[df['Procedure'] == 'Benchmark Heuristic']['MedianSplits'].values, color='g', width=barWidth, edgecolor='grey', label='Benchmark Heuristic')
# plt.bar(r2, df[df['Procedure'] == 'Two-Clause Heuristic']['MedianSplits'].values, color='b', width=barWidth, edgecolor='grey', label='Two-Clause Heuristic')
# plt.bar(r3, df[df['Procedure'] == 'Random Heuristic']['MedianSplits'].values, color='r', width=barWidth, edgecolor='grey', label='Random Heuristic')

for i, v in enumerate(bars_splits_benchmark):
    plt.bar(r1[i], v, color='g', width=barWidth, edgecolor='grey', label='Benchmark Heuristic' if i == 0 else "", hatch=hatch_patterns_splits_benchmark[i])

for i, v in enumerate(bars_splits_twoClause):
    plt.bar(r2[i], v, color='b', width=barWidth, edgecolor='grey', label='Two-Clause Heuristic' if i == 0 else "", hatch=hatch_patterns_splits_twoClause[i])

for i, v in enumerate(bars_splits_random):
    plt.bar(r3[i], v, color='r', width=barWidth, edgecolor='grey', label='Random Heuristic' if i == 0 else "", hatch=hatch_patterns_splits_random[i])


# Title & Subtitle
plt.title('Number of Splits vs L/N by Procedure (N=150)')
plt.xlabel('L/N Ratio', fontweight='bold')
plt.ylabel('Number of Splits (Median)')

# Add xticks on the middle of the grouped bars
plt.xticks([r + barWidth for r in range(len(ratios))], ratios)

# Create legend & Show graphic
plt.legend()
plt.grid(axis='y')
plt.tight_layout()
plt.savefig('number_of_splits_vs_L_N_grouped.png')
plt.clf()

# Plotting Probability of satisfiability vs L/N
plt.figure(figsize=(10, 6))
plt.bar(r1, df[df['Procedure'] == 'Benchmark Heuristic']['SatisfiabilityRatio'].values, color='g', width=barWidth, edgecolor='grey', label='Benchmark Heuristic')
plt.bar(r2, df[df['Procedure'] == 'Two-Clause Heuristic']['SatisfiabilityRatio'].values, color='b', width=barWidth, edgecolor='grey', label='Two-Clause Heuristic')
plt.bar(r3, df[df['Procedure'] == 'Random Heuristic']['SatisfiabilityRatio'].values, color='r', width=barWidth, edgecolor='grey', label='Random Heuristic')

# Title & Subtitle for the second graph
plt.title('Probability of Successful Satisfiability (Excluding TIMEOUTS) vs L/N by Procedure (N=150)')
plt.xlabel('L/N Ratio', fontweight='bold')
plt.ylabel('Probability of Satisfiability (Excluding Timeouts)')
plt.xticks([r + barWidth for r in range(len(ratios))], ratios)
plt.legend()
plt.grid(axis='y')
plt.tight_layout()
plt.savefig('probability_of_satisfiability_vs_L_N_grouped.png')
plt.clf()

# Ratio of the performance of heuristic to that of random-choice heuristic
# Initialize an empty list to store the ratios
performance_ratios_max_random = []
performance_ratios_max_twoClause = []

# Calculate the performance ratio for each L/N value
for ratio in ratios:
    benchmark_heuristic_time = df[(df['Procedure'] == 'Benchmark Heuristic') & (df['Ratio'] == ratio)]['MedianTime'].values[0]
    random_time = df[(df['Procedure'] == 'Random Heuristic') & (df['Ratio'] == ratio)]['MedianTime'].values[0]
    twoClause_time = df[(df['Procedure'] == 'Two-Clause Heuristic') & (df['Ratio'] == ratio)]['MedianTime'].values[0]

    # Compute the performance ratio
    performance_ratio_max_random = benchmark_heuristic_time / random_time
    performance_ratios_max_random.append(performance_ratio_max_random)

    performance_ratio_max_twoClause = benchmark_heuristic_time / twoClause_time
    performance_ratios_max_twoClause.append(performance_ratio_max_twoClause)

# Plotting
plt.figure(figsize=(10, 6))
plt.plot(ratios, performance_ratios_max_random, marker='o', linestyle='-')
plt.title('Performance Ratio of Benchmark Heuristic to random-choice vs L/N (N=150)')
plt.xlabel('L/N Ratio')
plt.ylabel('Performance Ratio (Benchmark Heuristic/random)')
plt.grid(axis='y')
plt.tight_layout()
plt.savefig('performance_ratio_max_random_vs_L_N.png')
plt.clf()

# Ratio of the performance of heuristic to that of 2-clause heuristic

plt.figure(figsize=(10, 6))
plt.plot(ratios, performance_ratios_max_twoClause, marker='o', linestyle='-')
plt.title('Performance Ratio of Benchmark Heuristic to two-clause vs L/N (N=150)')
plt.xlabel('L/N Ratio')
plt.ylabel('Performance Ratio (Benchmark Heuristic/twoClause)')
plt.grid(axis='y')
plt.tight_layout()
plt.savefig('performance_ratio_max_twoClause_vs_L_N.png')
plt.clf()
