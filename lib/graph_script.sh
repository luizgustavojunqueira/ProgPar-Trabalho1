#!/bin/bash

#compilar o código 
make main

#
./main ./graphs_datasets/citeseer.edgelist 3 1 >>citeseer.results
./main ./graphs_datasets/citeseer.edgelist 4 1 >>citeseer.results
./main ./graphs_datasets/citeseer.edgelist 5 1 >>citeseer.results
./main ./graphs_datasets/citeseer.edgelist 6 1 >>citeseer.results


    