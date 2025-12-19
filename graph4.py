import pandas as pd
import plotly.graph_objects as go
from plotly.subplots import make_subplots
import os

# ===== Load the four CSV files =====
df_std = pd.read_csv("sorting_results.csv")
df_merge = pd.read_csv("merge_results.csv")
df_ext = pd.read_csv("external_merge_results.csv")
df_pivot = pd.read_csv("quicksort_variants_results.csv")

# Ensure TimeSeconds is numeric
for dataframe in [df_std, df_merge, df_ext, df_pivot]:
    if 'TimeSeconds' in dataframe.columns:
        dataframe['TimeSeconds'] = pd.to_numeric(dataframe['TimeSeconds'], errors='coerce')

# ===== COLORS & LOGIC (Consistent with your reference) =====
palette = ['#1f77b4', '#ff7f0e', '#2ca02c', '#d62728', '#9467bd', '#8c564b', '#e377c2', '#7f7f7f']
cases = ['Random', 'SortedAsc', 'WorstCase']
algos_top = ["SelectionSort", "Insertion", "BubbleSort", "Quick"]
algo_merge_name = "merge"
algo_ext_name = "external-merge-2way"

# Build the unified color map
color_map = {}
all_algos_list = algos_top + [algo_merge_name, algo_ext_name] 
for i, algo in enumerate(all_algos_list):
    for j, case in enumerate(cases):
        color_map[(algo, case)] = palette[(i * len(cases) + j) % len(palette)]

pivot_colors = {
    "QuickFirst": "#1f77b4",
    "QuickLast": "#ff7f0e",
    "QuickMiddle": "#2ca02c",
    "QuickRandom": "#d62728"
}

# ===== Create 2x2 subplots =====
fig = make_subplots(
    rows=2, cols=2,
    specs=[[{"type": "xy"}, {"type": "xy"}],
           [{"type": "xy"}, {"type": "xy"}]],
    subplot_titles=("Standard Sorting Algorithms",
                    "External Merge Sort (Chunk vs Time)",
                    "Merge Sort Analysis",
                    "QuickSort Pivot Variants")
)

# ===== (1) Top-left: Standard Sorts =====
for algo in algos_top:
    for case in cases:
        df_f = df_std[(df_std['Algorithm'] == algo) & (df_std['Case'] == case)]
        if df_f.empty: continue
        color = color_map.get((algo, case), palette[0])
        
        fig.add_trace(go.Scatter(
            x=df_f['InputSize'], y=df_f['TimeSeconds'], mode='lines+markers', 
            name=f"{algo}-{case}", marker=dict(color=color), line=dict(color=color),
            legendgroup=f"{algo}-{case}", showlegend=True
        ), row=1, col=1)
        
        fig.add_trace(go.Bar(
            x=df_f['InputSize'], y=df_f['TimeSeconds'], 
            marker=dict(color=color), opacity=1.0, # <--- FIXED: Set to 1.0 for solid color
            legendgroup=f"{algo}-{case}", showlegend=False
        ), row=1, col=1)

# ===== (2) Top-right: External Merge =====
if not df_ext.empty:
    for input_size in sorted(df_ext['InputSize'].unique()):
        for case in df_ext['Case'].unique():
            df_f = df_ext[(df_ext['InputSize'] == input_size) & (df_ext['Case'] == case)].copy()
            if df_f.empty: continue
            df_f['ChunkStr'] = df_f['ChunkSize'].astype(str).str.replace('chunkSize=', '')
            
            fig.add_trace(go.Scatter(
                x=df_f['ChunkStr'], y=df_f['TimeSeconds'], mode='lines+markers',
                name=f"Ext-{case}-n={input_size}"
            ), row=1, col=2)
            
            fig.add_trace(go.Bar(
                x=df_f['ChunkStr'], y=df_f['TimeSeconds'],
                opacity=1.0 # <--- FIXED: Set to 1.0 for solid color
            ), row=1, col=2)

# ===== (3) Bottom-left: Merge Sort Only =====
for case in cases:
    df_f = df_merge[df_merge['Case'] == case]
    if df_f.empty: continue
    color = color_map.get((algo_merge_name, case), palette[0])
    
    fig.add_trace(go.Scatter(
        x=df_f['InputSize'], y=df_f['TimeSeconds'], mode='lines+markers',
        name=f"Merge-{case}", marker=dict(symbol="diamond", color=color),
        line=dict(color=color, dash="dot"), legendgroup=f"merge-{case}"
    ), row=2, col=1)
    
    fig.add_trace(go.Bar(
        x=df_f['InputSize'], y=df_f['TimeSeconds'], 
        marker=dict(color=color), opacity=1.0, # <--- FIXED: Set to 1.0
        legendgroup=f"merge-{case}", showlegend=False
    ), row=2, col=1)

# ===== (4) Bottom-right: Quick pivot variants =====
variant_col = 'Variant' if 'Variant' in df_pivot.columns else 'Algorithm'
for var in df_pivot[variant_col].unique():
    for case in df_pivot['Case'].unique():
        df_f = df_pivot[(df_pivot[variant_col] == var) & (df_pivot['Case'] == case)]
        if df_f.empty: continue
        color = pivot_colors.get(var, palette[0])
        
        fig.add_trace(go.Scatter(
            x=df_f['InputSize'], y=df_f['TimeSeconds'], mode='lines+markers',
            name=f"{var}-{case}", marker=dict(color=color), legendgroup=f"{var}-{case}"
        ), row=2, col=2)
        
        fig.add_trace(go.Bar(
            x=df_f['InputSize'], y=df_f['TimeSeconds'], 
            marker=dict(color=color), opacity=1.0, # <--- FIXED: Set to 1.0
            legendgroup=f"{var}-{case}", showlegend=False
        ), row=2, col=2)

# ===== Layout =====
fig.update_layout(
    height=1000, width=1600,
    title_text="Algorithm Performance Analysis (Solid Bars)",
    template="plotly_white",
    barmode='group'
)

# Set all Y-axes to Log
for i in range(1, 3):
    for j in range(1, 3):
        fig.update_yaxes(type='log', title_text="Time (s)", row=i, col=j)

fig.show()
