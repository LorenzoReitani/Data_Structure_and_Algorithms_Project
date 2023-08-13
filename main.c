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
struct Stazione{
    int kilometro;
    Macchina * rootMacchine;
    struct Stazione* next;
};
typedef struct Stazione Stazione;


/*#################################################################
 * dichiaro le funzioni
 #################################################################*/


//################ funzioni per le macchine #################

//funzione per creare un nuovo nodo maccchina
Macchina* CreateNodeMacchina(int autonomia) {
    Macchina* newNodeMacchina = (Macchina*)malloc(sizeof(Macchina));
    newNodeMacchina->autonomia = autonomia;
    newNodeMacchina->left = NULL;
    newNodeMacchina->right = NULL;
    return newNodeMacchina;
}

//funzione per aggiungere una macchina all'albero
Macchina* InsertNodeMacchina(Macchina** root, int value) {
    Macchina* prec = NULL;
    Macchina* corr = *root;
    while (corr != NULL){
        prec = corr;
        if(value<corr->autonomia){
            corr = corr->left;
        }else if(value>corr->autonomia){
            corr = corr->right;
        }else{
            corr->quantita += 1;
            printf("aggiunta\n");
            return *root;
        }
    }
    Macchina* nodoNuovo= CreateNodeMacchina(value);
    if( prec == NULL) {
        *root = nodoNuovo;
    }else if(value < prec->autonomia){
        prec->left = nodoNuovo;
    }else{
        prec->right = nodoNuovo;
    }
    printf("aggiunta\n");
    return *root;
}

// Funzione per trovare il valore massimo nel BST di macchine
int FindMaxMacchina(Macchina* root) {
    while (root->right != NULL) {
        root = root->right;
    }
    return root->autonomia;
}

//Funzione per cercare il padre una macchina con una certa autonomia
Macchina* CercaPadreMacchina(Macchina* root,int auton){
    Macchina* curr = root;
    Macchina *padre = NULL;
    while (curr != NULL && curr->autonomia != auton){
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
Macchina* SuccessoreMacchina(Macchina* root){
    Macchina* curr = root;
    curr = curr->right;
    while (curr->left != NULL){
        curr = curr->left;
    }
    return curr;
}

// Funzione per cancellare un nodo macchina nel BST
Macchina* DeleteNodeMacchina(Macchina** root, int value) {
    //creco il padre del nodo da cancellare
    Macchina* padre = CercaPadreMacchina(*root, value);
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
            printf("non rottamata\n");
            return *root;
        }
    }
    //se ci sono più macchine con la stesso autonomia riduco la quantità
    if(nodo->quantita>1){
        nodo->quantita-=1;
        printf("rottamata");
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
            //se il padre non c'è aggiorno la root
            else{
                *root = padre;
            }
            free(nodo);
            printf("rottamata");
        }
        //altrimenti ha 2 alberi e devo gestirli
        else{
            Macchina* successore = SuccessoreMacchina(nodo);
            int val= successore->autonomia;
            DeleteNodeMacchina(root, val);
            nodo->autonomia = val;
            }
    }
    return *root;
}

// Funzione per eliminaare un albero intero
Macchina* DeleteTreeMacchina(Macchina* root){
    if(root->right!=NULL)
        DeleteTreeMacchina(root->right);
    if(root->left!=NULL)
        DeleteTreeMacchina(root->left);
    free(root);
    return NULL;
}


//####################### funziomi per le stazioni ######################

//Funzione per creare una nuova stazione
Stazione* CreateStazione (int value){
    Stazione * newStazione = (Stazione *)malloc(sizeof(Stazione));
    newStazione->kilometro = value;
    newStazione->next = NULL;
    newStazione->rootMacchine = NULL;
    return newStazione;
}

//Funzione per inserire una nuova stazione
Stazione* InsertStazione(Stazione** testa, int value){
    Stazione* newStazione = CreateStazione(value);
    //se la testa è nulla la stazione diventa la testa
    if(*testa==NULL){
        *testa=newStazione;
    }
    else{
        //cerco dove posizionarla
        Stazione* corr = *testa;
        Stazione * prec = NULL;
        while(corr != NULL && corr->kilometro<value){
            prec = corr;
            corr = corr->next;
        }
        if (corr!= NULL && corr->kilometro == value){
            printf("non aggiunta\n");
            free(newStazione);
            return NULL;
        }else if(prec==NULL){
            //la stazione è la prima e diventa la testa
            newStazione->next=*testa;
            *testa = newStazione;
        }
        else{
            prec->next = newStazione;
            newStazione->next = corr;
        }
    }
    printf("aggiunta\n");
    return newStazione;
}

//Funzione per distruggere una stazione
Stazione* DeleteStazione(Stazione** testa, int value) {
    Stazione* corr= *testa;
    Stazione * prec=NULL;
    while(corr!=NULL && corr->kilometro!=value){
        prec=corr;
        corr=corr->next;
    }
    if(corr!= NULL){
        if(prec==NULL){
            *testa=corr->next;
        }else{
            prec->next=corr->next;
        }
        DeleteTreeMacchina(corr->rootMacchine);
        free(corr);
        printf("demolita\n");
    }else
        printf("non demolita\n");
    return *testa;
}


//##################### funzioni per cercare il percorso migliore ############################

//funzione per cercre il percorso al dritto
Stazione* CercaPercorso(Stazione* autostrada, int partenza, int arrivo, char* risultato){
    Stazione* stazione = autostrada;
    int autonomia = FindMaxMacchina(stazione->rootMacchine);
    int distanza = arrivo-partenza;
    if(distanza<autonomia){
        //ho trovato la stazione per arrivare

    }else{
        Stazione * corr = stazione;
        while(corr!=NULL){
            if(distanza)
        }
    }
}

int main() {
    Stazione* autostrada = NULL;
    Stazione* stazione = InsertStazione(&autostrada, 30);
    InsertNodeMacchina(&stazione->rootMacchine, 40);
    stazione = InsertStazione(&autostrada, 20);
    printf("20 = %i\n", stazione->kilometro);
    printf("20 = %i\n", autostrada->kilometro);
    InsertNodeMacchina(&stazione->rootMacchine, 10);
    InsertNodeMacchina(&stazione->rootMacchine, 15);
    InsertNodeMacchina(&stazione->rootMacchine, 5);
    InsertNodeMacchina(&stazione->rootMacchine, 25);
    int max = FindMaxMacchina(stazione->rootMacchine);
    printf("25 é %i\n", max);
    stazione = InsertStazione(&autostrada, 50);
    InsertNodeMacchina(&stazione->rootMacchine, 20);
    InsertNodeMacchina(&stazione->rootMacchine, 25);
    stazione = InsertStazione(&autostrada, 45);
    InsertNodeMacchina(&stazione->rootMacchine, 30);
    printf("45 = %i\n", stazione->kilometro);

    stazione=autostrada;
    while(stazione!=NULL){
        printf("%i(%i) -> ", stazione->kilometro, FindMaxMacchina(stazione->rootMacchine));
        stazione=stazione->next;
    }

    DeleteStazione(&autostrada,45);

    stazione=autostrada;
    while(stazione!=NULL){
        printf("%i(%i) -> ", stazione->kilometro, FindMaxMacchina(stazione->rootMacchine));
        stazione=stazione->next;
    }
    printf("\nfatto\n");

    DeleteNodeMacchina(&autostrada->next->next->rootMacchine, 250);
    max = FindMaxMacchina(autostrada->next->next->rootMacchine);
    printf("25 = %i\n", max);

}


