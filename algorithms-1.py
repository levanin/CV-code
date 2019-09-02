def pre_weighted_adjacency_list(graph_str):
    lines = str(graph_str).splitlines()
    params = lines[0].split()  # array of first line defining the graph's parameters
    isdirected = True if params[0] == "D" else False
    adjlist = [[] for _ in range(int(params[1]))]  # list of empty lists equal to the number of nodes
    for line in lines[1:]:
        numbers = [int(n) for n in line.split()]  # transition relation on each line
        has3 = True if len(numbers) == 3 else False
        for i in range(len(adjlist)):
            weight = numbers[2] if has3 == True else None
            if isdirected == True:
                if i == numbers[0]:
                    adjlist[i].append((numbers[1], 1))
            else:
                if i == numbers[0]:
                    adjlist[i].append((numbers[1], 1))
                if i == numbers[1]:
                    adjlist[i].append((numbers[0], 1))
    return adjlist


def dijkstra(adjlist, start):
    n = len(adjlist)
    intree = [False for _ in range(n)]
    distance = [float('inf') for _ in range(n)]
    parent = [None for _ in range(n)]
    distance[start] = 0
    for _ in range(n):
        if not all(intree):
            u = next_vertex(intree, distance)
            intree[u] = True
            for v, weight in adjlist[u]:
                if intree[v] is False and (distance[u] + weight) < distance[v]:
                    distance[v] = distance[u] + weight
                    parent[v] = u
    return parent, distance


def next_vertex(in_tree, distance):
    shortest = float('inf')
    location = -1
    for i in range(len(distance)):
        if shortest > distance[i] and in_tree[i] is False:
            shortest = distance[i]
            location = i
    return location


def shortest_path(parent, destination, start=0):
    current = destination
    path = [destination]

    n = len(parent)
    while parent[current] is not None:
        path.insert(0, parent[current])
        current = parent[current]
    return path if path[0] == start else None


def adapter_chain(adapters_info, charger_plug, wall_socket):
    adjlist = pre_weighted_adjacency_list(adapters_info)
    start = charger_plug
    destination = wall_socket
    parent, distance = dijkstra(adjlist, start)
    s = shortest_path(parent, destination, start)
    return "CS Unplugged!" if s == None else s

import collections
import copy
def adjacency_list(graph_str):
    lines = str(graph_str).splitlines()
    params = lines[0].split() # array of first line defining the graph's parameters
    isweighted = True if params[0] == "D" else False
    adjlist = [[] for _ in range(int(params[1]))]  # list of empty lists equal to the number of nodes
    for line in lines[1:]:
        numbers = [int(n) for n in line.split()] # transition relation on each line
        has3 = True if len(numbers) == 3 else False
        for i in range(len(adjlist)):
            weight = numbers[2] if has3 == True else None
            if isweighted == True:
                if i == numbers[0]:
                    adjlist[i].append(numbers[1])
            else:
                if i == numbers[0]:
                    adjlist[i].append(numbers[1])
                if i == numbers[1]:
                    adjlist[i].append(numbers[0])
    return adjlist


def bfs_loop(adj_list, Q, state):
    parent = [None for _ in range(len(adj_list))]
    while len(Q) != 0:
        u = Q.popleft()
        for v in adj_list[u]:
            if state[v] == "U":
                state[v] = "D"
                parent[v] = u
                Q.append(v)
        state[u] = "P"
    return state

def arrangement(direct_friendship_info):
    friendsrelation = adjacency_list(direct_friendship_info)  # friendship relations for each person
    n = len(friendsrelation)  # number of people
    state = ["U" for _ in range(n)]  # whether a friend is already in a particular table
    Q = collections.deque()
    tables = []
    for i in range (n):
        if state[i] == "U":
            previous = copy.deepcopy(state)
            state[i] = "D"
            Q.append(i)
            state = bfs_loop(friendsrelation, Q, state)
            newtable = [i for i in range(n) if state[i] != previous[i]]
            tables.append(newtable)
    return tables

def topo_loop(adj_list, u, state, order):
    state[u] = "D"
    for v in adj_list[u]:
        if state[v] == "D":
            return True
        if state[v] == 'U':
            topo_loop(adj_list, v, state, order)
    order.append(u)
    state[u] = 'P'
    return False


def topo_sort(adj_list):
    n = len(adj_list)
    order = []
    state = ['U' for _ in range(n)]
    for node in range(n):
        if state[node] == "U":
            cycle = topo_loop(adj_list, node, state, order)
        if cycle == True:
            break
    return order if cycle == False and 'D'  not in state else None


def computation_order(dependencies):
    adj_list = adjacency_list(dependencies)
    return topo_sort(adj_list)

def which_walkways(campus_map):
    adj_list = adjacency_list(campus_map)
    parent, _ = prims(adj_list, 0)
    return all_vertices(parent)

def all_vertices(parent):
    n = len(parent)
    vertices = []
    for u in range(n):
        if (parent[u] == None):
            continue
        elif (parent[u] > u):
            vertices.append((u, parent[u]))
        else:
            vertices.append((parent[u], u))
    return vertices


def prims(adjlist, start):
    n = len(adjlist)
    intree = [False for _ in range(n)]
    distance = [float('inf') for _ in range(n)]
    parent = [None for _ in range(n)]
    distance[start] = 0
    for _ in range(n):
        if not all(intree):
            u = next_vertex(intree, distance)
            intree[u] = True
            for v, weight in adjlist[u]:
                if intree[v] is False and (weight < distance[v]):
                    distance[v] = weight
                    parent[v] = u
    return parent, distance
def maximum_energy(city_map, depot_position):
    adjlist = adjacency_list(city_map)
    parent, distance = dijkstra(adjlist, depot_position)
    return longestrip(distance)


def longestrip(distance):
    noninf = [a for a in distance if a != float('inf')]
    return 2 * max(noninf)


def dijkstra(adjlist, start):
    n = len(adjlist)
    intree = [False for _ in range(n)]
    distance = [float('inf') for _ in range(n)]
    parent = [None for _ in range(n)]
    distance[start] = 0
    for _ in range(n):
        if not all(intree):
            u = next_vertex(intree, distance)
            intree[u] = True
            for v, weight in adjlist[u]:
                if intree[v] is False and (distance[u] + weight) < distance[v]:
                    distance[v] = distance[u] + weight
                    parent[v] = u
    return parent, distance
