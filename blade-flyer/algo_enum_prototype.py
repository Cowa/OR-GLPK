#!/usr/bin/python
# coding: utf-8

import collections

compare = lambda x, y: collections.Counter(x) == collections.Counter(y)

n  = 5
ca = 10
d  = [-1,2,4,2,4,2]
c  = [[0,334,262,248,277,302],[334,0,118,103,551,105],[262,118,0,31,517,180],[248,103,31,0,495,152],[277,551,517,495,0,476],[302,105,180,152,476,0]]

def algo_enum(enum, way, cursor, cap):
	if compare(way, [n]): # Cas terminal, ici si way == [5], alors on est sur le dernier chemin possible
		print(enum)

	elif cursor > n: # Si on dépasse les clients, on backtrack
		pop = way.pop()
		algo_enum(enum, way[:], pop+1, cap - d[pop])

	elif d[cursor] + cap <= ca: # On ajoute le client au chemin, et on ajoute le tout à l'énumération
		way.append(cursor)
		enum.append(way)
		algo_enum(enum, way[:], cursor+1, cap + d[cursor])

	else: # Capacité insuffisante, on tente le client prochain
		algo_enum(enum, way[:], cursor+1, cap)

algo_enum([], [], 1, 0)
