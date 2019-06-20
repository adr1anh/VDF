#%% dgs

import matplotlib.pyplot as plt
import datetime
import numpy as np
import pandas as pd
import matplotlib
import time

#%%
# modulus_data=pd.read_csv('graph_modulus.csv', sep=',')

# plt.close('all')
# fig, (ax1, ax2) = plt.subplots(1, 2, sharey=True)

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

# all_lines = []
# for key, grp in modulus_data[modulus_data['t'] >(2**21)].groupby(['modulus']):
#     if int(np.math.log2(key)) >= 12:
#         continue
#     l,=ax1.plot(grp['t'].apply(np.math.log2), grp['t_total'], label = "RSA{0:02d}".format(key), color=colors[key])         
#     ax1.plot(grp['t'].apply(np.math.log2), grp['t_proof'], color=colors[key], label='_nolegend_', linestyle='dotted')
#     ax1.plot(grp['t'].apply(np.math.log2), grp['t_squaring'], color=colors[key], label='_nolegend_', linestyle='dashed')
#     all_lines.extend([l])

# l1,=ax1.plot([], [], label='Total time', color='k')         
# l2,=ax1.plot([], [], label='Squaring time', linestyle='dashed', color='k')     
# l3,=ax1.plot([], [], label='Proof time', linestyle='dotted', color='k')   
# all_lines.extend([l1,l2,l3])  
# ax1.legend(handles=all_lines, loc=2)


# all_lines = []

# for key, grp in modulus_data[modulus_data['t'] >(2**16)].groupby(['modulus']):
#     if int(np.math.log2(key)) < 12:
#         continue
#     l, = ax2.plot(grp['t'].apply(np.math.log2), grp['t_total'], label = "RSA{0:02d}".format(key), color=colors[key])         
#     ax2.plot(grp['t'].apply(np.math.log2), grp['t_proof'], color=colors[key], label='_nolegend_', linestyle='dotted')
#     ax2.plot(grp['t'].apply(np.math.log2), grp['t_squaring'], color=colors[key], label='_nolegend_', linestyle='dashed')
#     all_lines.extend([l])
# ax2.legend(handles=all_lines, loc=2)

# formatter_time = matplotlib.ticker.FuncFormatter(lambda s, x: time.strftime('%M:%S', time.gmtime(s)))
# ax1.yaxis.set_major_formatter(formatter_time)
# ax2.yaxis.set_major_formatter(formatter_time)
# ax1.set_xlabel(r'$\log_2{t}$')
# ax2.set_xlabel(r'$\log_2{t}$')
# ax1.set_ylabel('Running time (minutes:seconds)')
# plt.tight_layout()

# fig.set_size_inches(10, 5)
# # fig.savefig('test2png.png', dpi=100)
# # plt.figure(num=None, figsize=(8, 6), dpi=80, facecolor='w', edgecolor='k')
# plt.savefig('modulus.pdf')
# plt.show()

# #%%

# overhead_data=pd.read_csv('graph_overhead.csv', sep=',')
# overhead_data.values



# fig, ax1 = plt.subplots()

# colors = {
#     0: 'b',
#     1: 'g',
#     2: 'r',
#     3: 'c',
#     4: 'm',
#     5: 'y',
#     6: 'k'
# }
# #     0: 'b',
# #     1: 'g',
# #     2: 'r',
# #     3: 'c',
# #     4: 'm',
# # }

# for key, grp in overhead_data.groupby(['segments']):
#     if key == 0:      
#         plt.plot(grp['modulus'].apply(np.math.log2), grp['t_squaring'], label='Time squaring', linestyle='dashed', color=colors[4])
#     plt.plot(grp['modulus'].apply(np.math.log2), grp['t_total'], label="Time w/ {0:0d} segment(s)".format(key+1), color=colors[key])   


# formatter_time = matplotlib.ticker.FuncFormatter(lambda s, x: time.strftime('%H:%M:%S', time.gmtime(s)))
# ax1.yaxis.set_major_formatter(formatter_time)
# ax1.set_xlabel('RSA Key Size')
# ax1.set_ylabel('Running time (hours:minutes:seconds)')
# plt.xticks(np.arange(8,16), 1 << np.arange(8,16))
# plt.legend(loc='best')
# plt.savefig('overhead.pdf')
# plt.show()


# #%%

# overheadopt_data=pd.read_csv('graph_overhead_opt.csv', sep=',')
# overheadopt_data.values


# np.sum(overheadopt_data['t_total'])
# fig, ax1 = plt.subplots()

# colors = {
#     0: 'b',
#     1: 'g',
#     2: 'r',
#     3: 'c',
#     4: 'm',
#     5: 'y',
#     6: 'k'
# }

# for key, grp in overheadopt_data.groupby(['segments']):
#     if key == 0:      
#         ax1.plot(grp['modulus'].apply(np.math.log2), grp['t_squaring'], label='Time squaring', linestyle='dashed', color=colors[0])
#     ax1.plot(grp['modulus'].apply(np.math.log2), grp['t_total'], label="Time w/ {} segment(s)".format(key+1), color=colors[key+1])   


# formatter_time = matplotlib.ticker.FuncFormatter(lambda s, x: time.strftime('%H:%M:%S', time.gmtime(s)))
# ax1.yaxis.set_major_formatter(formatter_time)
# ax1.set_xlabel('RSA Key Size')
# ax1.set_ylabel('Running time (hours:minutes:seconds)')
# plt.xticks(np.arange(8,16), 1 << np.arange(8,16))
# plt.legend(loc='best')

# # ax2 = ax1.twinx() 
# # ax2.set_ylabel(r'$\omega$', color='b')  # we already handled the x-label with ax1
# # for key, grp in overheadopt_data.groupby(['segments']):
# #     if key == 1:      
# #         ax2.plot(grp['modulus'].apply(np.math.log2), grp['overhead'], label='Time squaring', linestyle='dotted', color='b')

# # ax2.tick_params(axis='y', labelcolor='b', )



# plt.savefig('overheadopt.pdf')
# plt.show()


# #%%


# overheadopt_data=pd.read_csv('graph_overhead_opt.csv', sep=',')
# overhead_data=pd.read_csv('graph_overhead.csv', sep=',')

# plt.close('all')
# fig, (ax1, ax2) = plt.subplots(1, 2, sharey=True)

# for key, grp in overhead_data.groupby(['segments']):
#     if key == 0:      
#         ax1.plot(grp['modulus'].apply(np.math.log2), grp['t_squaring'], label='Time squaring', linestyle='dashed', color=colors[4])
#     ax1.plot(grp['modulus'].apply(np.math.log2), grp['t_total'], label="Time w/ {0:0d} segment(s)".format(key+1), color=colors[key])   


# ax1.legend(loc=2)


# for key, grp in overheadopt_data.groupby(['segments']):
#     if key == 0:      
#         ax2.plot(grp['modulus'].apply(np.math.log2), grp['t_squaring'], label='Time squaring', linestyle='dashed', color=colors[0])
#     ax2.plot(grp['modulus'].apply(np.math.log2), grp['t_total'], label="Time w/ {} segment(s)".format(key+1), color=colors[key+1])   

# # ax3 = ax2.twinx() 
# # ax3.set_ylabel(r'$\omega$', color='b')  # we already handled the x-label with ax1
# # for key, grp in overheadopt_data.groupby(['segments']):
# #     if key == 1:      
# #         ax3.plot(grp['modulus'].apply(np.math.log2), grp['overhead'], label='Time squaring', linestyle='dotted', color='b')


# formatter_time = matplotlib.ticker.FuncFormatter(lambda s, x: time.strftime('%M:%S', time.gmtime(s)))
# ax1.yaxis.set_major_formatter(formatter_time)
# ax2.yaxis.set_major_formatter(formatter_time)
# ax1.set_xlabel('RSA Key Size')
# ax2.set_xlabel('RSA Key Size')
# ax1.set_ylabel('Running time (hours:minutes:seconds)')
# plt.tight_layout()

# ax2.tick_params(axis='y', labelcolor='b', )
# ax2.legend(loc=2)

# ax1.set_xticks(np.arange(8,16),)
# ax1.set_xticklabels(1 << np.arange(8,16))
# ax2.set_xticks(np.arange(8,16),)
# ax2.set_xticklabels(1 << np.arange(8,16))

# fig.set_size_inches(10, 5)

# plt.savefig('overhead2.pdf')
# plt.show()


#%%


# plt.close('all')
# fig, (ax1, ax2) = plt.subplots(1, 2, sharey=True)
fig, ax1 = plt.subplots(1, 1, sharey=True)

overheadopt_data=pd.read_csv('graph_overhead_bar.csv', sep=',')

for n_segments in range(1,6):
    fig, ax1 = plt.subplots(1, 1, sharey=True)
    keysize = 4192
    for key, grp in overheadopt_data.groupby(['segments']):
        print(key)
        if int(key) == 0:
            ax1.axhline(y=grp.iloc[0]['t_total'])
            ax1.axhline(y=grp.iloc[0]['t_squaring'])

            # squaring_time = [grp['t_squaring'][0]]
            # proof_time = grp['t_squaring'][0]]
        if int(key)+1 == n_segments:
            # print(grp)
            ax1.axhline(y=grp.iloc[0]['t_total'])
            bottom_squaring = np.zeros(n_segments)
            squaring_time = np.zeros(n_segments)
            proof_time = np.zeros(n_segments)
            t_width = np.zeros(n_segments)
            t_index = np.zeros(n_segments)
            for i in range(n_segments):
                squaring_time[i] = grp.iloc[i]['t_squaring']
                proof_time[i] = grp.iloc[i]['t_proof']
                t_width[i] = grp.iloc[i]['t']
                for j in range(i+1, n_segments):
                    t_index[j] += grp.iloc[i]['t']
                    bottom_squaring[j] += grp.iloc[i]['t_squaring']


    p1 = ax1.bar(t_index, squaring_time, t_width, bottom=bottom_squaring, align='edge')
    p2 = ax1.bar(t_index, proof_time, t_width, bottom=bottom_squaring+squaring_time, align='edge')



    formatter_time = matplotlib.ticker.FuncFormatter(lambda s, x: time.strftime('%H:%M:%S', time.gmtime(s)))
    ax1.yaxis.set_major_formatter(formatter_time)
    ax1.set_xlabel('t')
    ax1.set_ylabel('Running time (hours:minutes:seconds)')
    # plt.xticks(np.arange(8,16), 1 << np.arange(8,16))
    # plt.legend(loc='best')


    # plt.ylabel('Scores')
    # plt.title('Scores by group and gender')
    # plt.xticks(ind, ('G1', 'G2', 'G3', 'G4', 'G5'))
    # plt.yticks(np.arange(0, 81, 10))
    # plt.legend((p1[0], p2[0]), ('Men', 'Women'))

    plt.show()

#%%
