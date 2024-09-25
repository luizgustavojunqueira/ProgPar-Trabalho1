#!/bin/bash

#compilar o código 
make main

# Executar o código para o dataset citeseer para os valores de k = 3, 4, 5 e 6 e alg = 1, 2 e 3
for alg in 1 2 
do
  for k in 3 4 5 6
    do
      ./main ./graph_datasets/citeseer.edgelist $k $alg >> citeseer.results
    done
done

