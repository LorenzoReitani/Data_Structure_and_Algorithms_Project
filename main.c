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

//Funzione per cercare il padre una macchina con una certa autonomia
Macchina* cercaPadreMacchina(Macchina* root,int auton){
    Macchina* curr = root;
    Macchina *padre = NULL;
    while (curr->autonomia != auton && curr != NULL){
        padre = curr;
        if (auton < curr->autonomia){
            curr = curr->left;
        }else{
            curr = curr->right;
        }
    }
    return padre;
}

// Funzione per trovare il successore di una macchina
Macchina* successreMacchina(Macchina* root, int value){
    Macchina* curr = root;
    curr = curr->right;
    while (curr->left != NULL){
        curr = curr->left;
    }
    return curr;
}


// Funzione per cancellare un nodo macchina nel BST
Macchina* deleteNodeMacchina(Macchina** root, int value) {
    //creco il padre del nodo da cancellare
    Macchina* padre = cercaPadreMacchina(*root, value);
    Macchina* nodo = NULL;
    //se il padre é NULL vuol dire che il nodo è la radice
    if(padre == NULL){
        nodo = *root;
    }else{
        // se il padre ha almeno un figlio vuol dire che il nodo cercato esiste
        if (padre->right!=NULL || padre->left!=NULL){
            if (value<padre->autonomia){
                nodo = padre->left;
            }else{
                nodo = padre->right;
            }
        }
        //altrimenti non è stato trovato
        else{
            return *root;
        }
    }
    //se ci sono più macchine con la stesso autonomia riduco la quantità
    if(nodo->quantita>1){
        nodo->quantita-=1;
        return *root;
    }
    //se c'è una macchina sola devo togliere il nodo
    else{
        Macchina *puntatore;
        //se il nodo ha al massimo 1 figlio
        if(nodo->right==NULL || nodo->left==NULL){
            if(nodo->right==NULL){
                puntatore=nodo->left;
            }else{
                puntatore=nodo->right;
            }
            //se il padre c'è lo aggiorno
            if(padre!=NULL){
                if(value<padre->autonomia){
                    padre->left=puntatore;
                }else{
                    padre->right=puntatore;
                }
            }
            //se il padre non c'è aggiotno la root
            else{
                *root = padre;
            }
            free(nodo);
        }
        //altrimenti ha 2 alberi e devo gestirli
        else{

        }
    }
}


int main() {
    printf("Hello, World!\n");
    return 0;
}


