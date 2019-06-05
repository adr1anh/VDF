#%% dgs

import matplotlib.pyplot as plt
import datetime
import numpy as np
import pandas as pd
import matplotlib
import time


# modulus_data=pd.read_csv('graph_modulus.csv', sep=',')

# fig, ax1 = plt.subplots()

# colors = {
#     256: 'b',
#     512: 'g',
#     1024: 'r',
#     2048: 'c',
#     4096: 'm',
#     8192: 'y',
#     16384: 'k',
#     32768: 'b'
# }

# l7 = []
# for key, grp in modulus_data.groupby(['modulus']):
#     if key == 256:
#         l1,=plt.plot(grp['t'].apply(np.math.log2), grp['t_total'], label='Total time', color=colors[key])         
#         l2,=plt.plot(grp['t'].apply(np.math.log2), grp['t_squaring'], label='Squaring time', linestyle='dashed', color=colors[key])
#         l3,=plt.plot(grp['t'].apply(np.math.log2), grp['t_proof'], label='Proof time', linestyle='dotted', color=colors[key])
#     else:
#         l4,=plt.plot(grp['t'].apply(np.math.log2), grp['t_total'], label = "RSA{0:02d}".format(key), color=colors[key])         
#         l5,=plt.plot(grp['t'].apply(np.math.log2), grp['t_proof'], color=colors[key], label='_nolegend_', linestyle='dotted')
#         l6,=plt.plot(grp['t'].apply(np.math.log2), grp['t_squaring'], color=colors[key], label='_nolegend_', linestyle='dashed')
#         l7.extend([l4])

# first_legend = plt.legend(handles=[l1,l2,l3], loc=1)
# plt.gca().add_artist(first_legend)
# plt.legend(handles=l7, loc=2)
# ax1 = plt.gca()
# formatter_time = matplotlib.ticker.FuncFormatter(lambda s, x: time.strftime('%M:%S', time.gmtime(s)))
# ax1.yaxis.set_major_formatter(formatter_time)
# ax1.set_xlabel(r'$\log_2{t}$')
# ax1.set_ylabel('Running time (minutes:seconds)')

# plt.savefig('modulus.pdf')
# plt.show()

# #%%

# overhead_data=pd.read_csv('/Users/Adrian/Desktop/graph_overhead.csv', sep=',')
# overhead_data.values



# fig, ax1 = plt.subplots()

# colors = {
#     0: 'b',
#     1: 'g',
#     2: 'r',
#     3: 'c',
#     4: 'm',
# }

# for key, grp in overhead_data.groupby(['segments']):
#     if key == 0:      
#         plt.plot(grp['modulus'].apply(np.math.log2), grp['t_squaring'], label='Time squaring', linestyle='dashed', color=colors[4])
#     plt.plot(grp['modulus'].apply(np.math.log2), grp['t_total'], label="Time w/ {0:0d} segment(s)".format(key+1), color=colors[key])   


# formatter_time = matplotlib.ticker.FuncFormatter(lambda s, x: time.strftime('%M:%S', time.gmtime(s)))
# ax1.yaxis.set_major_formatter(formatter_time)
# ax1.set_xlabel('RSA Key Size')
# ax1.set_ylabel('Running time (minutes:seconds)')
# plt.xticks(np.arange(8,16), 1 << np.arange(8,16))
# plt.legend(loc='best')
# plt.savefig('overhead.pdf')
# plt.show()


#%%

overheadopt_data=pd.read_csv('/Users/Adrian/Desktop/graph_overhead_opt.csv', sep=',')
overheadopt_data.values



fig, ax1 = plt.subplots()

colors = {
    0: 'b',
    1: 'g',
    2: 'r',
    3: 'c',
    4: 'm',
    5: 'y'
}

for key, grp in overheadopt_data.groupby(['segments']):
    if key == 0:      
        ax1.plot(grp['modulus'].apply(np.math.log2), grp['t_squaring'], label='Time squaring', linestyle='dashed', color=colors[4])
    ax1.plot(grp['modulus'].apply(np.math.log2), grp['t_total'], label="Time w/ {} segment(s)".format(key+1), color=colors[key])   


formatter_time = matplotlib.ticker.FuncFormatter(lambda s, x: time.strftime('%M:%S', time.gmtime(s)))
ax1.yaxis.set_major_formatter(formatter_time)
ax1.set_xlabel('RSA Key Size')
ax1.set_ylabel('Running time (minutes:seconds)')
plt.xticks(np.arange(8,16), 1 << np.arange(8,16))
plt.legend(loc='best')

ax2 = ax1.twinx() 
ax2.set_ylabel(r'$\omega$', color='y')  # we already handled the x-label with ax1
for key, grp in overheadopt_data.groupby(['segments']):
    if key == 1:      
        ax2.plot(grp['modulus'].apply(np.math.log2), grp['overhead'], label='Time squaring', color=colors[5])

ax2.tick_params(axis='y', labelcolor='y')



plt.savefig('overhead.pdf')
plt.show()


#%%
