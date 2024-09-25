#!/bin/bash

#compilar o código 
make main

# Executar o código para o dataset citeseer para os valores de k = 3, 4, 5 e 6 e alg = 1, 2 e 3

echo " " >> cliques.results
echo "==============================">> cliques.results
date +"%d/%m/%Y %H:%M:%S">> cliques.results
echo "Executando o código para o dataset citeseer"

echo "citeseer" >> cliques.results
for alg in 1 2 
do

  if [ $alg -eq 1 ];
  then
    echo "Executando o algoritmo sequencial"
  else
    if [ $alg -eq 2 ];
    then
      echo "Executando o algoritmo paralelo"
    else 
      echo "Executando o algoritmo paralelo com balanceamento de carga"
    fi
  fi


  for k in 3 4 5 6
    do
      echo "Executando para k = $k"
      ./main ./graph_datasets/citeseer.edgelist $k $alg >> cliques.results
    done
done

echo ""

echo "Executando o código para o dataset ca_astroph"

echo "ca_astroph" >> cliques.results
# Executar o código para o dataset ca_astroph para os valores de k = 3, 4, 5, 6 e 7, e alg = 1, 2 e 3
for alg in 1 2
do

  if [ $alg -eq 1 ];
  then
    echo "Executando o algoritmo sequencial"
  else
    if [ $alg -eq 2 ];
    then
      echo "Executando o algoritmo paralelo"
    else 
      echo "Executando o algoritmo paralelo com balanceamento de carga"
    fi
  fi

  for k in 3 4 5 6 7
    do
      echo "Executando para k = $k"
      ./main ./graph_datasets/ca_astroph.edgelist $k $alg >> cliques.results
    done
done

echo ""

echo "Executando o código para o dataset dblp"

echo "dblp" >> cliques.results
# Executar o código para o dataset dblp para os valores de k = 3, 4, 5 e 6 e alg = 1, 2 e 3
for alg in 1 2
do

  if [ $alg -eq 1 ];
  then
    echo "Executando o algoritmo sequencial"
  else
    if [ $alg -eq 2 ];
    then
      echo "Executando o algoritmo paralelo"
    else 
      echo "Executando o algoritmo paralelo com balanceamento de carga"
    fi
  fi

  for k in 3 4 5 6
    do
      echo "Executando para k = $k"
      ./main ./graph_datasets/dblp.edgelist $k $alg >> cliques.results
    done
done
