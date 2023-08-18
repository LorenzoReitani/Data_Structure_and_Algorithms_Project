#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>



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
    struct Stazione* precedente;
    struct Stazione* next;
};
typedef struct Stazione Stazione;

//dichiaro la struttura nodo della tappa per ricercare il percorso
struct NodeTappa{
    Stazione * stazione;
    struct NodeTappa* precedente;
    struct NodeTappa** arrivi;
};
typedef struct NodeTappa Tappa;


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
    newStazione->precedente=NULL;
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
            //la stazione già esiste, non aggiungo nulla
            printf("non aggiunta\n");
            free(newStazione);
            return NULL;
        }else if(prec==NULL){
            //la stazione è la prima e diventa la testa
            newStazione->next=*testa;
            *testa = newStazione;
        }
        else{
            //aggiungo la stazione nella lista
            prec->next = newStazione;
            newStazione->precedente=prec;
            newStazione->next = corr;
            if(corr!=NULL) {
                corr->precedente = newStazione;
            }
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
            //devo eliminare la testa
            *testa=corr->next;
            (corr->next)->precedente = NULL;
        }else{
            prec->next=corr->next;
            (corr->next)->precedente = prec;
        }
        DeleteTreeMacchina(corr->rootMacchine);
        free(corr);
        printf("demolita\n");
    }else
        //stazione non trovata
        printf("non demolita\n");
    return *testa;
}

//####################### funziomi per le tappe ######################
Tappa* CreateTappa(Stazione* staz){
    Tappa* newTappa = (Tappa*)malloc(sizeof (Tappa));
    newTappa->precedente=NULL;
    newTappa->stazione=staz;
    newTappa->arrivi=NULL;
    return newTappa;
}


//##################### funzioni per cercare il percorso migliore ############################

//funzione che trova gli arrivi partendo da una posizione
Tappa** CercaArrivi(Tappa* partenza, int arrivo, Stazione* raggiunta, bool* trovato, int* size){
    int distanza = arrivo-partenza->stazione->kilometro;
    int autonomia =  FindMaxMacchina(partenza->stazione->rootMacchine);
    Tappa** arrivi = NULL;
    int lunghezzaArrivi = 0;
    if(distanza< autonomia){
        //se la distanza dall'arrivo è minore dell'autonomia ho trovato il percorso migliore
        *trovato=true;
        return arrivi;
    }else{
        //altrimenti mi salvo le tappe dove sono arrivato;
        Stazione * corr = raggiunta->next;
        while(corr->kilometro - partenza->stazione->kilometro < autonomia){
            lunghezzaArrivi++;
            Tappa* newTappa = CreateTappa(corr);
            newTappa->precedente=partenza;
            arrivi=(Tappa**) realloc(arrivi, lunghezzaArrivi* sizeof(Tappa*));
            arrivi[lunghezzaArrivi-1]=newTappa;
            corr = corr->next;
        }
        *size=lunghezzaArrivi;
        return arrivi;
    }
}

//funzione che trova gli arrivi a ritroso partendo da una posizione
Tappa** CercaArriviRitroso(Tappa* partenza, int arrivo, Stazione* raggiunta, bool* trovato, int* size){
    int distanza = (partenza->stazione->kilometro)-arrivo;
    int autonomia =  FindMaxMacchina(partenza->stazione->rootMacchine);
    Tappa** arrivi = NULL;
    int lunghezzaArrivi = 0;
    if(distanza< autonomia){
        //se la distanza dall'arrivo è minore dell'autonomia ho trovato il percorso migliore
        *trovato=true;
        return arrivi;
    }else{
        //altrimenti mi salvo le tappe dove sono arrivato;
        Stazione * corr = raggiunta->next;
        while(partenza->stazione->kilometro - corr->kilometro < autonomia){
            lunghezzaArrivi++;
            Tappa* newTappa = CreateTappa(corr);
            newTappa->precedente=partenza;
            arrivi=(Tappa**) realloc(arrivi, lunghezzaArrivi* sizeof(Tappa*));
            arrivi[lunghezzaArrivi-1]=newTappa;
            corr = corr->precedente;
        }
        *size=lunghezzaArrivi;
        return arrivi;
    }
}

//funzione per scrivere il percorso
char* ScriviPercorso(Tappa* fine,int arrivo ,int profondità){
    int* tappe = (int*)malloc((profondità) * sizeof(int));
    Tappa* corr = fine;
    for(int i = profondità-1; i>=0 ; i--){
        tappe[i]=corr->stazione->kilometro;
        corr=corr->precedente;
    }
    char* risultato = (char*)malloc(7*sizeof (char)*(profondità-1));
    strcpy(risultato,"");
    //scrivo il ciclo per scrivere il risultato fino alla penultima tappa
    char num[10];
    for(int i=0; i<= profondità-1; i++) {
        sprintf(num, "%d", tappe[i]);
        strcat(risultato,num);
        strcat(risultato, " -> ");
    }
    //aggiungo al risultato la tappa finale senza le frecce
    sprintf(num, "%d", arrivo);
    strcat(risultato, num);
    return risultato;
}

//funzione che calcola il percorso
char* CalcolaPercorso(Stazione* autostrada, int start, int end){
    bool trovato = false;
    Stazione* partenza = autostrada;
    while(partenza->kilometro!=start){
        partenza=partenza->next;
    }
    Tappa* rootTappa = CreateTappa(partenza);
    Tappa** arrivi =(Tappa**) malloc(sizeof (Tappa*));
    Tappa* conclusione=NULL;
    arrivi[0]=rootTappa;
    int lunghezzaArrivi = 1;
    Stazione* raggiunta = partenza;
    int profondità = 0;
    while(!trovato){
        Tappa** newArrivi = (Tappa**) malloc(0);
        int lunghezzaNewArrivi=0;
        for(int i=0;i< lunghezzaArrivi ; i++){
            int lunghezzaArriviParziali=0;
            Tappa** arriviParziali = CercaArrivi(arrivi[i], end, raggiunta, &trovato, &lunghezzaArriviParziali);
            if(trovato){
                conclusione=arrivi[i];
            }else {
                //ciclo for per aggiungere gli arrivi parziali ai nuovi arrivi
                lunghezzaNewArrivi += lunghezzaArriviParziali;
                newArrivi = (Tappa **) realloc(newArrivi, lunghezzaNewArrivi * sizeof(Tappa *));
                for (int j = lunghezzaNewArrivi - lunghezzaArriviParziali; i < lunghezzaNewArrivi; i++) {
                    newArrivi[j] = arriviParziali[j - (lunghezzaNewArrivi - lunghezzaArriviParziali)];
                }
                raggiunta=arriviParziali[lunghezzaArriviParziali-1]->stazione;
            }
            free(arriviParziali);
        }
        free(arrivi);
        arrivi = newArrivi;
        lunghezzaArrivi=lunghezzaNewArrivi;
        profondità++;
    }

    char* percorso = ScriviPercorso(conclusione,end ,profondità);
    free(arrivi);
    return percorso;
}

//funzione per calcolare il percorso a ritroso

char* CalcolaPercorsoRitroso(Stazione* sedere, int start, int end){
    bool trovato = false;
    Stazione* partenza = sedere;
    while(partenza->kilometro!=start){
        partenza=partenza->precedente;
    }
    Tappa* rootTappa = CreateTappa(partenza);
    Tappa** arrivi =(Tappa**) malloc(sizeof (Tappa*));
    Tappa* conclusione=NULL;
    arrivi[0]=rootTappa;
    int lunghezzaArrivi = 1;
    Stazione* raggiunta = partenza;
    int profondità = 0;
    while(!trovato){
        Tappa** newArrivi = (Tappa**) malloc(0);
        int lunghezzaNewArrivi=0;
        for(int i=0;i< lunghezzaArrivi ; i++){
            int lunghezzaArriviParziali=0;
            Tappa** arriviParziali = CercaArrivi(arrivi[i], end, raggiunta, &trovato, &lunghezzaArriviParziali);
            if(trovato){
                conclusione=arrivi[i];
            }else {
                //ciclo for per aggiungere gli arrivi parziali ai nuovi arrivi
                lunghezzaNewArrivi += lunghezzaArriviParziali;
                newArrivi = (Tappa **) realloc(newArrivi, lunghezzaNewArrivi * sizeof(Tappa *));
                for (int j = lunghezzaNewArrivi - lunghezzaArriviParziali; i < lunghezzaNewArrivi; i++) {
                    newArrivi[j] = arriviParziali[j - (lunghezzaNewArrivi - lunghezzaArriviParziali)];
                }
                raggiunta=arriviParziali[lunghezzaArriviParziali-1]->stazione;
            }
            free(arriviParziali);
        }
        free(arrivi);
        arrivi = newArrivi;
        lunghezzaArrivi=lunghezzaNewArrivi;
        profondità++;
    }

    char* percorso = ScriviPercorso(conclusione,end ,profondità);
    free(arrivi);
    return percorso;
}
/*########################################################################
 * main del programma
 ########################################################################*/

int main() {
/*
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

    char* percorso = CalcolaPercorso(autostrada, 20, 50);
    printf("\n%s\n", percorso);
*/
    //########################################
    Stazione* autostrada = NULL;
    FILE* finput= fopen("test.text", "r");
    FILE* foutput=fopen("risultato.text","w");
    char letto[20];
    fscanf(finput, "%s ", letto);
    int numero;
    while(!feof(finput)) {
        switch (letto[0]) {
            case 'a':
                if(letto[9]=='s'){
                    //caso di aggiungi-stazione
                    fscanf(finput, "%i", &numero);
                    Stazione* stazione = InsertStazione(&autostrada, numero);
                    if(stazione!=NULL) {
                        fscanf(finput, "%i", &numero);
                        int aut;
                        for (int i = numero; i > 0; i--) {
                            fscanf(finput, "%i", &aut);
                            InsertNodeMacchina(&(stazione->rootMacchine), aut);
                        }
                    }
                }else{
                    fscanf(finput, "%i", &numero);
                    int autonomia;
                    fscanf(finput, "%i", &autonomia);
                    Stazione* stazione = autostrada;
                    while(stazione!=NULL && stazione->kilometro != numero){
                        stazione = stazione->next;
                    }
                    if(stazione!=NULL) {
                        InsertNodeMacchina(&stazione->rootMacchine, autonomia);
                    }
                }
                break;

            case 'd':
                //caso demolisci-stazione
                fscanf(finput, "%i", &numero);
                DeleteStazione(&autostrada, numero);
                break;

            case 'r':
                //caso rottama-auto
                fscanf(finput, "%i", &numero);
                int autonomia;
                fscanf(finput, "%i", &autonomia);
                Stazione* stazione = autostrada;
                while(stazione!=NULL && stazione->kilometro != numero){
                    stazione = stazione->next;
                }
                if(stazione!=NULL) {
                    DeleteNodeMacchina(&stazione->rootMacchine, autonomia);
                }
                break;

            case 'p':
                //caso pianifica-percorso;
                fscanf(finput, "%i", &numero);
                int fine;
                fscanf(finput, "%i", &fine);
                if(numero == fine){
                    //stampa solo il numero
                }else {
                    char *percorso;
                    if (numero < fine) {
                        //cerco il percorso al dritto
                        percorso = CalcolaPercorso(autostrada, numero, fine);
                    }else{
                        percorso = CalcolaPercorsoRitroso(autostrada, numero, fine);
                        //cerco il perocrso a ritroso
                    }
                    printf("\n%s\n", percorso);
                }
                break;

            default:
                break;
        }
    }
    /*
    Stazione** arrivi;
    stazione=autostrada;
    int lunghezza=0;
    while(stazione!=NULL){
        lunghezza +=1;
        arrivi = (Stazione**) realloc(arrivi, sizeof(Stazione*)*lunghezza);
        arrivi[lunghezza-1]=stazione;
        //printf(" size = %lu\n", sizeof(arrivi));
        printf("%i(%i) -> ", stazione->kilometro, FindMaxMacchina(stazione->rootMacchine));
        stazione=stazione->next;
    }
    printf("\n");

    for(int i=0; i<lunghezza; i+=1){
        printf("%i(%i) -> ", arrivi[i]->kilometro, FindMaxMacchina(arrivi[i]->rootMacchine));
        //free(arrivi[i]);
    }

    printf("\npuntatore autostrada: %p\n", autostrada);
    printf("puntatore arrivi: %p\n", arrivi);
    printf("puntatore arrivi[0]: %p\n", arrivi[0]);

    printf("\npuntatore autostrada: %p\n", &autostrada);
    printf("puntatore arrivi: %p\n", &arrivi);
    printf("puntatore arrivi[0]: %p\n", &arrivi[0]);

    printf("\n");
    free(arrivi);

    stazione=autostrada;
    while(stazione!=NULL){
        printf("%i(%i) -> ", stazione->kilometro, FindMaxMacchina(stazione->rootMacchine));
        stazione=stazione->next;
    }*/

/*
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
*/
}


