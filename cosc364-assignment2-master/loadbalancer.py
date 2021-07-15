import cplex
import sys

"""
Authors: Shai Levin, Ryan Miller

Load Balancer for a network with X input nodes, Y transit nodes and Z destination nodes.
First creates a LP file, then solves it using python's cplex module.

Run in terminal: python3 loadbalancer.py X Y Z
"""


def create_lp_file(x, y, z, filename):
    #   constructs the lp file given x source nodes, y transit nodes, and z destination nodes, into file filename
    xmatrix = variable_matrix("x", x, y, z)  # xijk = the load of each individual path from i, through j to k
    umatrix = variable_matrix("u", x, y, z)  # uijk = 0 or 1 if the path ijk is used

    lp_file = open(filename, "w")
    lp_file.write("Minimize\n"
                  "r\n")
    #  CONSTRAINT EQUATIONS HERE
    constraints = "Subject to\n"

    # the sum of costs all paths from i->j through all nodes k is equal to i + j
    for i in range(x):
        for j in range(z):
            constraints += "demandflow{}to{}: ".format((i + 1), (j + 1))
            for k in range(y):
                if k > 0:
                    constraints += " + "
                constraints += xmatrix[i][k][j]
            demandvolume = (i + 1) + (j + 1)
            constraints += " = " + str(demandvolume) + "\n"


    # the number of paths used from nodes i->j through all nodes k is equal to 2
    for i in range(x):
        for j in range(z):
            constraints += "usedpaths{}to{}: ".format((i + 1), (j + 1))
            for k in range(y):
                if k > 0:
                    constraints += " + "
                constraints += umatrix[i][k][j]
            constraints += " = 2\n"

    # individual path costs dependent on demandvolume and constraints
    for i in range(x):
        for k in range(z):
            for j in range(y):
                constraints += "pathcost{}to{}to{}: ".format((i + 1), (j + 1), (k + 1))
                constraints += xmatrix[i][j][k] + " - " + str(((i + k + 2) / 2)) + " " + \
                               umatrix[i][j][k] + " = 0\n"

    #Sum the cost of paths across each transit node to derive load on transit node
    for k in range(y):
        constraints += "loadacrossnode{}: ".format(k + 1)
        for i in range(x):
            for j in range(z):
                if (i != 0 or j != 0):
                    constraints += " + "
                constraints += xmatrix[i][k][j]
        constraints += " - T{} = 0\n".format(k+1)

    # Constraint variable r which results in load balancing
    for k in range(y):
        constraints += "constrainnode{} : T{} - r <= 0\n".format(k+1, k+1)

    # the source->transit link cost calculation dependent on xijk values.
    for i in range(x):
        for j in range(y):
            constraints += "linkcostC{}to{}: ".format((i + 1), (j + 1))
            for k in range(z):
                if k > 0:
                    constraints += " + "
                constraints += xmatrix[i][j][k]
            constraints += " - " + variable("C", i + 1, j + 1, "") + " = 0\n"

    # the transit->dest link cost calculation dependent on xijk values.
    for j in range(y):
        for k in range(z):
            constraints += "linkcostD{}to{}: ".format((j + 1), (k + 1))
            for i in range(x):
                if i > 0:
                    constraints += " + "
                constraints += xmatrix[i][j][k]
            constraints += " - " + variable("D", j + 1, k + 1, "") + " = 0\n"

    # BOUNDS HERE
    bounds = "Bounds\n"
    for i in range(x):
        for j in range(y):
            for k in range(z):
                bounds += xmatrix[i][j][k] + " >= 0\n"
    # BINARY HERE
    binary = "Binary\n"
    for i in range(x):
        for j in range(y):
            for k in range(z):
                binary += umatrix[i][j][k] + "\n"

    lp_file.write(constraints + bounds + binary + "End")
    lp_file.close()


def variable_matrix(a, x, y, z):
    # return a 3d array of permutations aijk of [i],[j],[k]
    matrix = [[[None for _ in range(z)] for _ in range(y)] for _ in range(x)]
    for i in range(x):
        for j in range(y):
            for k in range(z):
                matrix[i][j][k] = variable(a, i + 1, j + 1, k + 1)
    return matrix


def variable(a, i, j, k):
    # return a string aijk
    return str(a) + str(i) + str(j) + str(k)


def solve_lp_file(filename):
    c = cplex.Cplex(filename)  # create a cplex LP with filename

    # Disable all logging from cplex to stdout
    c.set_log_stream(None)
    c.set_error_stream(None)
    c.set_warning_stream(None)
    c.set_results_stream(None)
    start = c.get_time()
    c.solve()  # solve LP
    duration = c.get_time() - start
    outputs = list(zip(c.variables.get_names(), c.solution.get_values()))
    transit_loads = [value for value in outputs if "T" in value[0]]
    linkcosts = [value[1] for value in outputs if ("C" in value[0] or "D" in value[0]) and value[1] > 0.01]
    capacity = max(linkcosts)
    usedlinks = len(linkcosts)

    print("For (X,Y,Z) = ({},{},{})".format(sys.argv[1], sys.argv[2], sys.argv[3]))
    print("=========================")
    print("Runtime : {} seconds".format(duration))
    print("Load on Transit Nodes:")
    for transit_load in transit_loads:
        print("{} : {}".format(transit_load[0], transit_load[1]))
    print("Capacity of link with highest capacity : {:.2f}".format(capacity))
    print("Number of links Cij or Dij utilized : {}".format(usedlinks))


filename = "loadbalancer.lp"
create_lp_file(int(sys.argv[1]), int(sys.argv[2]), int(sys.argv[3]), filename)
solve_lp_file(filename)
