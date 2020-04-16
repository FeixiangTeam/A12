import os
import json
import csv
import numpy as np
import sys

def analysis(exec_file, input_file, k = 0):
	res = {'id': k}
	paths = json.load(os.popen(f'{exec_file} {input_file}'))
	res['total distance'] = round(sum(item['distance'] for item in paths), 8)
	ratios = [item['ratio'] for item in paths]
	res['ratio minimum'] = round(min(ratios), 8)
	res['ratio maximum'] = round(max(ratios), 8)
	res['ratio average'] = round(np.mean(ratios), 8)
	res['ratio variance'] = round(np.var(ratios), 8)
	trucks = [item['truck'] for item in paths]
	truck_num = {}
	for truck in set(trucks):
		truck_num[truck] = trucks.count(truck)
	res['truck usage'] = truck_num
	return res

_, input_file, output_file, exec_file, times = sys.argv
print(f'analysing {exec_file}')

with open(output_file, 'w') as csvfile:
	fieldnames = ['id', 'total distance', 'ratio minimum', 'ratio maximum', 'ratio average', 'ratio variance', 'truck usage']
	writer = csv.DictWriter(csvfile, fieldnames = fieldnames)

	writer.writeheader()
	for i in range(int(times)):
		print(f'at task {i}')
		writer.writerow(analysis(exec_file, input_file, k = i))
