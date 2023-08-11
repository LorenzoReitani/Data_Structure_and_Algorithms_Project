#include <stdio.h>
#include <stdlib.h>


/*######################################################################
 * dichiaro le strutture dati
 ###################################àà#################################*/


//dichiaro la struttura nodo della macchina per il BST
struct NodeMacchina {
    int autonomia;
    int quantita;
    struct NodeMacchina* left;
    struct NodeMacchina* right;
};
typedef struct NodeMacchina Macchina;

//dichiaro la struttura della stazione
typedef struct {
    int kilometro;
    Macchina * rootMacchine;
}Stazione ;


/*#################################################################
 * dichiaro le funzioni
 #################################################################*/


//funzione per creare un nuovo nodo maccchina
Macchina* createNodeMacchina(int autonomia) {
    Macchina* newNodeMacchina = (Macchina*)malloc(sizeof(Macchina));
    newNodeMacchina->autonomia = autonomia;
    newNodeMacchina->left = NULL;
    newNodeMacchina->right = NULL;
    return newNodeMacchina;
}

//funzione per aggiungere una macchina all'albero
Macchina* insertNodeMacchina(Macchina* root, int value) {
    Macchina* prec = NULL;
    Macchina* corr = root;
    while (corr != NULL){
        prec = corr;
        if(value<corr->autonomia){
            corr = corr->left;
        }else if(value>corr->autonomia){
            corr = corr->right;
        }else{
            corr->quantita += 1;
            return root;
        }
    }
    Macchina* nodoNuovo= createNodeMacchina(value);
    if( prec == NULL) {
        root = nodoNuovo;
    }else if(value < prec->autonomia){
        prec->left = nodoNuovo;
    }else{
        prec->right = nodoNuovo;
    }
    return root;
}

// Funzione per trovare il valore massimo nel BST di macchine
int findMaxMacchina(Macchina* root) {
    while (root->right != NULL) {
        root = root->right;
    }
    return root->autonomia;
}

// Funzione per cancellare un nodo macchina nel BST
Macchina* deleteNodeMacchina(Macchina* root, int value) {

}


int main() {
    printf("Hello, World!\n");
    return 0;
}


