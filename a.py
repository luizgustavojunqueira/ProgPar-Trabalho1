
import random


def generate_random_edge_list(num_vertices, num_edges):
  """Generates a random edge list for a graph.

  Args:
    num_vertices: The number of vertices in the graph.
    num_edges: The number of edges in the graph.

  Returns:
    A list of tuples representing the edges.
  """

  edges = []
  while len(edges) < num_edges:
    v1 = random.randint(0, (num_vertices**2))
    v2 = random.randint(0, (num_vertices**2))
    if v1 != v2 and (v1, v2) not in edges and (v2, v1) not in edges:
      edges.append((v1, v2))
  return edges

edge_list = generate_random_edge_list(10, 20)

# Print the edge list
for edge in edge_list:
  print(edge[0], edge[1])
