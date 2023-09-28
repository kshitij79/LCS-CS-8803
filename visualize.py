import pandas as pd
import matplotlib.pyplot as plt
import numpy as np

# Read data from CSV
df = pd.read_csv('expt_results.csv')

# Extract relevant data
ratios = df['Ratio'].unique()
procedures = df['Procedure'].unique()

# Setup bar width and positions
barWidth = 0.25
r1 = np.arange(len(ratios))
r2 = [x + barWidth for x in r1]
r3 = [x + barWidth for x in r2]

# Create bars for each procedure
plt.bar(r1, df[df['Procedure'] == 'maxLiteral']['MedianTime'].values, color='g', width=barWidth, edgecolor='grey', label='maxLiteral')
plt.bar(r2, df[df['Procedure'] == 'twoClause']['MedianTime'].values, color='b', width=barWidth, edgecolor='grey', label='twoClause')
plt.bar(r3, df[df['Procedure'] == 'random']['MedianTime'].values, color='r', width=barWidth, edgecolor='grey', label='random')

# Title & Subtitle
plt.title('Compute Time vs L/N by Procedure')
plt.xlabel('L/N Ratio', fontweight='bold')
plt.ylabel('Compute Time (Median)')

# Add xticks on the middle of the grouped bars
plt.xticks([r + barWidth for r in range(len(ratios))], ratios)

# Create legend & Show graphic
plt.legend()
plt.grid(axis='y')
plt.tight_layout()
plt.savefig('compute_time_vs_L_N_grouped.png')


# Plotting Number of DPLL calls vs L/N
plt.bar(r1, df[df['Procedure'] == 'maxLiteral']['MedianSplits'].values, color='g', width=barWidth, edgecolor='grey', label='maxLiteral')
plt.bar(r2, df[df['Procedure'] == 'twoClause']['MedianSplits'].values, color='b', width=barWidth, edgecolor='grey', label='twoClause')
plt.bar(r3, df[df['Procedure'] == 'random']['MedianSplits'].values, color='r', width=barWidth, edgecolor='grey', label='random')

# Title & Subtitle
plt.title('Number of Splits vs L/N by Procedure')
plt.xlabel('L/N Ratio', fontweight='bold')
plt.ylabel('Number of Splits (Median)')

# Add xticks on the middle of the grouped bars
plt.xticks([r + barWidth for r in range(len(ratios))], ratios)

# Create legend & Show graphic
plt.legend()
plt.grid(axis='y')
plt.tight_layout()
plt.savefig('number_of_splits_vs_L_N_grouped.png')

# Plotting Probability of satisfiability vs L/N
plt.figure(figsize=(10, 6))
plt.bar(r1, df[df['Procedure'] == 'maxLiteral']['SatisfiabilityRatio'].values, color='g', width=barWidth, edgecolor='grey', label='maxLiteral')
plt.bar(r2, df[df['Procedure'] == 'twoClause']['SatisfiabilityRatio'].values, color='b', width=barWidth, edgecolor='grey', label='twoClause')
plt.bar(r3, df[df['Procedure'] == 'random']['SatisfiabilityRatio'].values, color='r', width=barWidth, edgecolor='grey', label='random')

# Title & Subtitle for the second graph
plt.title('Probability of Satisfiability vs L/N by Procedure')
plt.xlabel('L/N Ratio', fontweight='bold')
plt.ylabel('Probability of Satisfiability without Timeout')
plt.xticks([r + barWidth for r in range(len(ratios))], ratios)
plt.legend()
plt.grid(axis='y')
plt.tight_layout()
plt.savefig('probability_of_satisfiability_vs_L_N_grouped.png')

# Ratio of the performance of heuristic to that of random-choice heuristic
# Initialize an empty list to store the ratios
performance_ratios_max_random = []
performance_ratios_max_twoClause = []

# Calculate the performance ratio for each L/N value
for ratio in ratios:
    maxLiteral_time = df[(df['Procedure'] == 'maxLiteral') & (df['Ratio'] == ratio)]['MedianTime'].values[0]
    random_time = df[(df['Procedure'] == 'random') & (df['Ratio'] == ratio)]['MedianTime'].values[0]
    twoClause_time = df[(df['Procedure'] == 'twoClause') & (df['Ratio'] == ratio)]['MedianTime'].values[0]

    # Compute the performance ratio
    performance_ratio_max_random = maxLiteral_time / random_time
    performance_ratios_max_random.append(performance_ratio_max_random)

    performance_ratio_max_twoClause = maxLiteral_time / twoClause_time
    performance_ratios_max_twoClause.append(performance_ratio_max_twoClause)

# Plotting
plt.figure(figsize=(10, 6))
plt.plot(ratios, performance_ratios_max_random, marker='o', linestyle='-')
plt.title('Performance Ratio of maxLiteral to random-choice vs L/N')
plt.xlabel('L/N Ratio')
plt.ylabel('Performance Ratio (maxLiteral/random)')
plt.grid(axis='y')
plt.tight_layout()
plt.savefig('performance_ratio_max_random_vs_L_N.png')

# Ratio of the performance of heuristic to that of 2-clause heuristic

plt.figure(figsize=(10, 6))
plt.plot(ratios, performance_ratios_max_twoClause, marker='o', linestyle='-')
plt.title('Performance Ratio of maxLiteral to two-clause vs L/N')
plt.xlabel('L/N Ratio')
plt.ylabel('Performance Ratio (maxLiteral/twoClause)')
plt.grid(axis='y')
plt.tight_layout()
plt.savefig('performance_ratio_max_twoClause_vs_L_N.png')