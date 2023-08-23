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
/*struct Stazione{
    int kilometro;
    Macchina * rootMacchine;
    struct Stazione* precedente;
    struct Stazione* next;
};
typedef struct Stazione Stazione;
*/
//dichiaro la struttura della stazione come albero
struct StazioneNodo{
    int kilometro;
    Macchina * rootMacchine;
    struct StazioneNodo* precedente;
    struct StazioneNodo* next;
    struct StazioneNodo* padre;
    struct StazioneNodo* right;
    struct StazioneNodo* left;
};
typedef struct StazioneNodo StazioneNodo;

//dichiaro la struttura nodo della tappa per ricercare il percorso
struct NodeTappa{
    StazioneNodo * stazione;
    struct NodeTappa* precedente;
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
    newNodeMacchina->quantita=1;
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
    return *root;
}

// Funzione per trovare il valore massimo nel BST di macchine
int FindMaxMacchina(Macchina* root) {
    if(root==NULL){
        return 0;
    }
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

// Funzione per cancellare il successore nel BST
Macchina* DeleteSuccessore(Macchina** root, int value) {
    //creco il padre del nodo da cancellare
    Macchina* padre = CercaPadreMacchina(*root, value);
    Macchina* nodo = NULL;
    //se il padre é NULL vuol dire che il nodo è la radice
    if(padre == NULL){
        nodo = *root;
    }else{
        // se il padre ha almeno un figlio vuol dire che il nodo cercato esiste
        if (padre->right!=NULL && padre->right->autonomia==value){
            nodo = padre->right;
        }else if(padre->left != NULL && padre->left->autonomia==value){
            nodo = padre->left;
        }else{
            //altrimenti non è stato trovato
            printf("non rottamata\n");
            return *root;
        }
    }
    //se c'è una macchina sola devo togliere il nodo
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
            *root = puntatore;
        }
        free(nodo);
        printf("rottamata\n");
    }
    return *root;
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
        if (padre->right!=NULL && padre->right->autonomia==value){
            nodo = padre->right;
        }else if(padre->left != NULL && padre->left->autonomia==value){
            nodo = padre->left;
        }else{
            //altrimenti non è stato trovato
            printf("non rottamata\n");
            return *root;
        }
    }
    //se ci sono più macchine con la stesso autonomia riduco la quantità
    if(nodo->quantita>1){
        nodo->quantita-=1;
        printf("rottamata\n");
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
                *root = puntatore;
            }
            free(nodo);
            printf("rottamata\n");
        }
            //altrimenti ha 2 alberi e devo gestirli
        else{
            Macchina* successore = SuccessoreMacchina(nodo);
            int val= successore->autonomia;
            int quant = successore->quantita;
            DeleteSuccessore(root, val);
            nodo->autonomia = val;
            nodo->quantita = quant;
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

/*//Funzione per creare una nuova stazione
Stazione* CreateStazione (int value){
    Stazione * newStazione = (Stazione *)malloc(sizeof(Stazione));
    newStazione->kilometro = value;
    newStazione->precedente=NULL;
    newStazione->next = NULL;
    newStazione->rootMacchine = NULL;
    return newStazione;
}*/

//Funzione per creare una nuova stazione nodo
StazioneNodo* CreateStazioneNodo (int value){
    StazioneNodo* newStazione = (StazioneNodo *)malloc(sizeof(StazioneNodo));
    newStazione->kilometro = value;
    newStazione->precedente=NULL;
    newStazione->next = NULL;
    newStazione->rootMacchine = NULL;
    newStazione->left=NULL;
    newStazione->right=NULL;
    newStazione->padre=NULL;
    return newStazione;
}

//Funzione per cercare una stazione
StazioneNodo* CercaStazione(StazioneNodo* root ,int value){
    StazioneNodo* corr = root;
    while(corr!=NULL && corr->kilometro!=value) {

        if (value < corr->kilometro) {
            corr = corr->left;
        } else {
            corr = corr->right;
        }
    }
    return corr;
}

/*//Funzione per inserire una nuova stazione
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
            (*testa)->precedente = newStazione;
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
}*/

//Funzione per inserire una stazione nodo nell'albero
StazioneNodo* InsertStazioneNodo(StazioneNodo** root, int value){
    StazioneNodo* prec = NULL;
    StazioneNodo* corr = *root;
    while (corr != NULL){
        prec = corr;
        if(value<corr->kilometro){
            corr = corr->left;
        }else if(value>corr->kilometro){
            corr = corr->right;
        }else{
            printf("non aggiunta\n");
            return NULL;
        }
    }
    StazioneNodo* newStazione = CreateStazioneNodo(value);
    if(prec==NULL){
        //l'autostrada è vuota e devo inizializzarla
        *root = newStazione;
    }else if(value < prec->kilometro){
        prec->left = newStazione;
    }else{
        prec->right = newStazione;
    }

    newStazione->padre=prec;
    StazioneNodo* successivo = newStazione;
    while (successivo!=NULL && successivo->kilometro <= value){
        successivo = successivo->padre;
    }
    newStazione->next = successivo;
    if(successivo!=NULL) {
        newStazione->precedente = successivo->precedente;
        if(successivo->precedente != NULL) {
            (successivo->precedente)->next = newStazione;
        }
        successivo->precedente = newStazione;
    }else{
        newStazione->precedente = newStazione->padre;
        if(newStazione->padre!=NULL) {
            newStazione->padre->next = newStazione;
        }
    }
    printf("aggiunta\n");
    return newStazione;
}

/*//Funzione per distruggere una stazione
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
            if(corr->next != NULL) {
                (corr->next)->precedente = NULL;
            }
        }else{
            prec->next=corr->next;
            if(corr->next!=NULL) {
                (corr->next)->precedente = prec;
            }
        }
        DeleteTreeMacchina(corr->rootMacchine);
        free(corr);
        printf("demolita\n");
    }else
        //stazione non trovata
        printf("non demolita\n");
    return *testa;
}*/

//funzione per gestire la cancellazione di un nodo con due figli
StazioneNodo* DeleteConSuccessore(StazioneNodo* root) {
    StazioneNodo *successore = root->right;
    while (successore->left != NULL) {
        successore = successore->left;
    }
    if (successore->padre != root) {
        successore->padre->left = successore->right;


        if (successore->right != NULL) {
        successore->right->padre = successore->padre;
        }

        root->right->padre=successore;


        successore->right=root->right;
    }
    successore->padre=root->padre;
    if(root->padre!=NULL) {
        if(root->kilometro < root->padre->kilometro){
            root->padre->left=successore;
        }else{
            root->padre->right=successore;
        }
    }
    if(root->left != NULL){
        root->left->padre=successore;
    }
    successore->left=root->left;
    free(root);
    return successore;
}

//funzione per cancellare un nodo stazione
StazioneNodo* DeleteStazioneNodo(StazioneNodo** root, int value){
    StazioneNodo* nodo = CercaStazione(*root, value);
    if(nodo!=NULL){
        if(nodo->padre == NULL){
            //se il padre é NULL vuol dire che il nodo è la radice
            nodo = *root;
        }
        //aggiorno precedente e successivo
        if(nodo->next != NULL){
            nodo->next->precedente=nodo->precedente;
        }
        if(nodo->precedente != NULL){
            nodo->precedente->next=nodo->next;
        }
        //distruggo il parcheggio
        DeleteTreeMacchina(nodo->rootMacchine);
        //procedo ad eliminare il nodo
        StazioneNodo* puntatore;
        //se il nodo ha al massimo 1 figlio
        if(nodo->right==NULL || nodo->left==NULL) {
            if (nodo->right == NULL) {
                puntatore = nodo->left;
            } else {
                puntatore = nodo->right;
            }
            //se il padre c'è lo aggiorno
            if(nodo->padre!=NULL){
                if(value < nodo->padre->kilometro){
                    nodo->padre->left=puntatore;
                }else{
                    nodo->padre->right=puntatore;
                }
                if(puntatore!=NULL) {
                    puntatore->padre = nodo->padre;
                }
            }
            //se il padre non c'è aggiorno la root
            else{
                *root = puntatore;
                if(puntatore!=NULL) {
                    puntatore->padre = NULL;
                }
            }
            free(nodo);
        }
        //altrimenti ha 2 alberi e devo gestirli
        else{
            DeleteConSuccessore(nodo);
        }
        printf("demolita\n");
    }else{
        //stazione non trovata
        printf("non demolita\n");
    }
    return *root;
}

//####################### funziomi per le tappe ######################
Tappa* CreateTappa(StazioneNodo* staz){
    Tappa* newTappa = (Tappa*)malloc(sizeof (Tappa));
    newTappa->precedente=NULL;
    newTappa->stazione=staz;
    return newTappa;
}


//##################### funzioni per cercare il percorso migliore ############################

//funzione che trova gli arrivi partendo da una posizione
Tappa** CercaArrivi(Tappa* partenza, int arrivo, StazioneNodo* raggiunta, bool* trovato, int* size){
    int distanza = arrivo-partenza->stazione->kilometro;
    int autonomia =  FindMaxMacchina(partenza->stazione->rootMacchine);
    Tappa** arrivi = NULL;
    int lunghezzaArrivi = 0;
    if(distanza<= autonomia){
        //se la distanza dall'arrivo è minore dell'autonomia ho trovato il percorso migliore
        *trovato=true;
        return arrivi;
    }else{
        //altrimenti mi salvo le tappe dove sono arrivato;
        StazioneNodo * corr = raggiunta->next;
        while(corr->kilometro - partenza->stazione->kilometro <= autonomia){
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
Tappa** CercaArriviRitroso(Tappa* partenza, int arrivo, StazioneNodo* raggiunta, bool* trovato, int* size){
    int distanza = (partenza->stazione->kilometro)-arrivo;
    int autonomia =  FindMaxMacchina(partenza->stazione->rootMacchine);
    Tappa** arrivi = NULL;
    int lunghezzaArrivi = 0;
    if(distanza<= autonomia){
        //se la distanza dall'arrivo è minore dell'autonomia ho trovato il percorso migliore
        *trovato=true;
        return arrivi;
    }else{
        //altrimenti mi salvo le tappe dove sono arrivato;
        StazioneNodo * corr = raggiunta->precedente;
        while(((partenza->stazione->kilometro) - (corr->kilometro)) <= autonomia){
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
char* ScriviPercorso(Tappa* fine,int arrivo ,int profondita){
    int* tappe = (int*)malloc((profondita) * sizeof(int));
    Tappa* corr = fine;
    for(int i = profondita-1; i>=0 ; i--){
        tappe[i]=corr->stazione->kilometro;
        corr=corr->precedente;
    }
    char* risultato = (char*)malloc(10*sizeof (char)*(profondita-1));
    strcpy(risultato,"");
    //scrivo il ciclo per scrivere il risultato fino alla penultima tappa
    char num[20];
    for(int i=0; i<= profondita-1; i++) {
        sprintf(num, "%d ", tappe[i]);
        strcat(risultato,num);
    }
    //aggiungo al risultato la tappa finale senza le frecce
    sprintf(num, "%d", arrivo);
    strcat(risultato, num);
    return risultato;
}

//funzione che calcola il percorso
char* CalcolaPercorso(StazioneNodo* autostrada, int start, int end){
    bool trovato = false;
    StazioneNodo* partenza = CercaStazione(autostrada, start);
    Tappa* rootTappa = CreateTappa(partenza);
    Tappa** arrivi =(Tappa**) malloc(sizeof (Tappa*));
    Tappa* conclusione=NULL;
    arrivi[0]=rootTappa;
    int lunghezzaArrivi = 1;
    StazioneNodo* raggiunta = partenza;
    int profondita = 0;
    while(!trovato){
        Tappa** newArrivi = (Tappa**) malloc(0);
        int lunghezzaNewArrivi=0;
        for(int i=0;i< lunghezzaArrivi && !trovato ; i++){
            int lunghezzaArriviParziali=0;
            Tappa** arriviParziali = CercaArrivi(arrivi[i], end, raggiunta, &trovato, &lunghezzaArriviParziali);
            if(trovato){
                conclusione=arrivi[i];
            }else {
                //ciclo for per aggiungere gli arrivi parziali ai nuovi arrivi
                lunghezzaNewArrivi += lunghezzaArriviParziali;
                newArrivi = (Tappa **) realloc(newArrivi, lunghezzaNewArrivi * sizeof(Tappa *));
                for (int j = lunghezzaNewArrivi - lunghezzaArriviParziali; j < lunghezzaNewArrivi; j++) {
                    newArrivi[j] = arriviParziali[j - (lunghezzaNewArrivi - lunghezzaArriviParziali)];
                }
                if (lunghezzaArriviParziali > 0) {
                    raggiunta = arriviParziali[lunghezzaArriviParziali - 1]->stazione;
                }
            }
            free(arriviParziali);
        }
        free(arrivi);
        arrivi = newArrivi;
        lunghezzaArrivi=lunghezzaNewArrivi;
        profondita++;
        if(lunghezzaArrivi==0 && !trovato){
            free(arrivi);
            return NULL;
        }
    }

    char* percorso = ScriviPercorso(conclusione,end ,profondita);
    free(arrivi);
    return percorso;
}

//funzione per calcolare il percorso a ritroso

char* CalcolaPercorsoRitroso(StazioneNodo* autostrada, int start, int end){
    bool trovato = false;
    StazioneNodo* partenza = CercaStazione(autostrada,start);
    Tappa* rootTappa = CreateTappa(partenza);
    Tappa** arrivi =(Tappa**) malloc(sizeof (Tappa*));
    Tappa* conclusione=NULL;
    arrivi[0]=rootTappa;
    int lunghezzaArrivi = 1;
    StazioneNodo* raggiunta = partenza;
    int profondita = 0;
    while(!trovato){
        Tappa** newArrivi = (Tappa**) malloc(0);
        int lunghezzaNewArrivi=0;
        for(int i=lunghezzaArrivi-1; i>=0 && !trovato; i--){
            int lunghezzaArriviParziali=0;
            Tappa** arriviParziali = CercaArriviRitroso(arrivi[i], end, raggiunta, &trovato, &lunghezzaArriviParziali);
            if(trovato){
                conclusione=arrivi[i];
                i=-1;
            }else {
                //ciclo for per aggiungere gli arrivi parziali ai nuovi arrivi
                lunghezzaNewArrivi += lunghezzaArriviParziali;
                newArrivi = (Tappa **) realloc(newArrivi, lunghezzaNewArrivi * sizeof(Tappa *));
                for (int j = lunghezzaNewArrivi - lunghezzaArriviParziali; j < lunghezzaNewArrivi; j++) {
                    newArrivi[j] = arriviParziali[j - (lunghezzaNewArrivi - lunghezzaArriviParziali)];
                }
                if(lunghezzaArriviParziali>0) {
                    raggiunta = arriviParziali[lunghezzaArriviParziali - 1]->stazione;
                }
            }
            free(arriviParziali);
        }
        free(arrivi);
        arrivi = newArrivi;
        lunghezzaArrivi=lunghezzaNewArrivi;
        profondita++;
        if(lunghezzaArrivi==0 && !trovato){
            free(arrivi);
            return NULL;
        }
    }

    char* percorso = ScriviPercorso(conclusione,end ,profondita);
    free(arrivi);
    return percorso;
}
/*########################################################################
 * main del programma
 ########################################################################*/

int main() {
    StazioneNodo* autostrada = NULL;
    char letto[20];
    int numero;
    int scan=1;
    //int linea=1;
    scan=scanf("%s ", letto);
    while(!feof(stdin)) {
        //printf("%d\n", linea);
        //linea++;
        switch (letto[0]) {
            case 'a':
                if(letto[9]=='s'){
                    //caso di aggiungi-stazione
                    scan=scanf( "%i", &numero);
                    StazioneNodo* stazione = InsertStazioneNodo(&autostrada, numero);
                    if(stazione!=NULL) {
                        scan = scanf( "%i", &numero);
                        int aut;
                        for (int i = numero; i > 0; i--) {
                            scan = scanf( "%i", &aut);
                            InsertNodeMacchina(&(stazione->rootMacchine), aut);
                        }
                    }
                }else{
                    //caso di aggiungi-auto
                    scan = scanf( "%i", &numero);
                    int autonomia;
                    scan = scanf( "%i", &autonomia);
                    StazioneNodo* stazione = CercaStazione(autostrada, numero);
                    if(stazione!=NULL) {
                        InsertNodeMacchina(&stazione->rootMacchine, autonomia);
                        printf("aggiunta\n");
                    }else{
                        printf("non aggiunta\n");
                    }
                }
                break;

            case 'd':
                //caso demolisci-stazione
                scan = scanf( "%i", &numero);
                DeleteStazioneNodo(&autostrada, numero);
                break;

            case 'r':
                //caso rottama-auto
                scan=scanf( "%i", &numero);
                int autonomia;
                scan=scanf( "%i", &autonomia);
                StazioneNodo * stazione = CercaStazione(autostrada, numero);
                if(stazione!=NULL) {
                    DeleteNodeMacchina(&stazione->rootMacchine, autonomia);
                }
                else{
                    printf("non rottamata\n");
                }
                break;

            case 'p':
                //caso pianifica-percorso;
                scan=scanf( "%i", &numero);
                int fine;
                scan=scanf( "%i", &fine);
                if(numero == fine){
                    printf("%i\n", fine);
                }else {
                    char *percorso;
                    if (numero < fine) {
                        //cerco il percorso al dritto
                        percorso = CalcolaPercorso(autostrada, numero, fine);
                    }else{
                        percorso = CalcolaPercorsoRitroso(autostrada, numero, fine);
                        //cerco il perocrso a ritroso
                    }
                    if(percorso == NULL){
                        printf("nessun percorso\n");
                    }else {
                        printf("%s\n", percorso);
                        free(percorso);
                    }
                }
                break;

            default:
                //linea--;
                break;
        }
        scan=scanf("%s ", letto);
    }
    if(!scan){}
}


