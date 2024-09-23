#!/bin/bash

#compilar o código 
make main

#
./main ./graph_datasets/citeseer.edgelist 3 1 >>citeseer.results
./main ./graph_datasets/citeseer.edgelist 4 1 >>citeseer.results
./main ./graph_datasets/citeseer.edgelist 5 1 >>citeseer.results
./main ./graph_datasets/citeseer.edgelist 6 1 >>citeseer.results


