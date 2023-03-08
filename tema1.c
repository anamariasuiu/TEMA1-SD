#include<stdio.h>
#include<stdlib.h>
#include<string.h>
// structura pentru lista
typedef struct node {
    char data[500];
    char ch;
    struct node *prev;
    struct node *next;
} node_t;
// structura pentru stiva
typedef struct nod {
   struct nod *next;
   char data[500];
} dequeue_t; 
// functie pentru initializarea stiva
void alloc_deq(dequeue_t **deq){
    *deq = NULL;
}
// functie pentru initializare lista
void init(node_t **head){
    *head = (node_t*)malloc(sizeof(node_t));
    (*head)->next = NULL;
    (*head)->prev = NULL;
}
void print_sir(node_t *head){
    while(head!=NULL){
        printf("%s", head->data);
        head = head ->next;
    }
}
void print_ch(node_t *head){
    while(head != NULL){
        printf("%c", head->ch);
        head = head ->next;
    }
}
void print_deq(dequeue_t **stiva){
    dequeue_t *stack = *stiva;
    while(stack!= NULL){
        printf("%s", stack->data);
        stack = stack->next;    
    }
    free(stack);
}
// functie pentru inserare nod la sfarsitul listei
void push_back(node_t **head, node_t *new_node) {
    node_t *p = *head;
    while(p->next!=NULL) {
        p = p->next;
    }
    p->next = new_node;
    new_node->prev = p;
    new_node->next = NULL;    
    
} 
// functie pentru inserare nod in stiva
dequeue_t *push_deq(dequeue_t **deq, dequeue_t *new_node) {
    if(*deq == NULL){
        *deq = new_node;
        return new_node;
    }
    new_node->next = *deq; 
    *deq = new_node;
    return new_node;
}
// functie pentru eliminare nod din lista
void remove_node(node_t **node) {
    node_t *back, *after,*p = *node;
    back = (*node)->prev;

    if((*node)->next != NULL) 
    after = (*node)->next;
    else after = NULL;

    if(after != NULL){
    back->next = after;
    after->prev = back;
    } else back->next = NULL;

    *node = after;
    free(p);
}
// functie pentru salvarea textului caracter cu caracter in fisier
void save(node_t **head, node_t **cursor, FILE *fo) {
    fo = fopen("editor.out", "w");
    node_t *p = *head;
    node_t *c = *cursor;
    char caracter;
    p = p->next;

    while(p->next != NULL && p != c) {
        caracter = p->ch;
        fputc(caracter, fo);
        p = p->next;
     }
     if( p->prev->ch != '\n'&& p->ch == '\n') {
        caracter = p->ch;
        fputc(caracter, fo);
        }
        if(p == c || c->prev != NULL){
            if(c->ch == '\0')
            c = c->next;
            while(c->next != NULL) {
                caracter = c->ch;
                fputc(caracter, fo);
                c = c->next; 
                }
                if(c->ch == '\n' && c->prev->ch != '\n' && c->prev->ch != '\0' ) {
                    caracter = c->ch;
                    fputc(caracter, fo);
                    }
    }
    fclose(fo);
}
// functie pentru mutarea cursorului la inceputul unei linii
node_t  *gl_line(node_t **head, char *sir) {
    node_t *cursor = *head;
    int *nr_linie;
    nr_linie = (int*)malloc(2 *sizeof(int));

    if(strlen(sir) <= 5)
    sscanf(&sir[3], "%d", nr_linie);
    else sscanf(&sir[5], "%d", nr_linie);
    
    cursor = cursor->next;
    for(int i = 0; i < *nr_linie - 1; i++) {
        while(cursor->ch != '\n') {
            cursor = cursor->next;
        }
        //sarim si \n
        cursor = cursor->next;
    }
    free(nr_linie);
    return cursor;
}
// functie pentru stergerea unei linii
void dl_line(node_t **cursor, node_t **head, char sir[]) {
    if(strlen(sir) > 3 ) {
       *cursor = gl_line(head,sir);
    }
     do {
         remove_node(cursor);
          } while((*cursor)->ch != '\n');
            //stergem si \n
             remove_node(cursor);
}
// functie pentru stergerea caracterului dinaintea cursorului
void backspace(node_t **cursor){
    node_t *back_character = (*cursor)->prev;
    remove_node(&back_character);    
}
/* functie pentru mutarea cursorului pe o anumita linie, dupa un
anumitor caracter */
void gc_char_line(node_t **cursor, node_t **head, char sir[]) {
    if(strlen(sir) > 5)
    *cursor = gl_line(head, sir);
    int pozitie;
    sscanf(&sir[3], "%d", &pozitie);

    for(int i = 0; i < pozitie ; i++)
    *cursor = (*cursor)->next;
}
// functia delete
void d_chars(node_t **cursor, node_t **head, char sir[]){
    int nr_caractere;
    if(strlen(sir) == 2)
    nr_caractere = 1;
    else sscanf(&sir[2], "%d", &nr_caractere);

    if((*cursor)->ch == '\n')
    *cursor = (*cursor)->next;

    for(int i = 0; i < nr_caractere; i++)
    remove_node(cursor); 
}
// functie pentru inlocuirea unui sir cu alt sir
void re_old_new(node_t **cursor, char sir[]){
    char *token, delim[2] = " \n", *old_word, *new_word, old[20], new[20];
    int ok = 1, i = 0, j, k = 0;
    token = strtok(sir,delim);

     while(k < 2 && token != NULL ){
         token = strtok(NULL, delim);

         if(k == 0)
         old_word = token;
         else new_word = token;

          k++;
          }  
          strcpy(old, old_word);
          strcpy(new, new_word);
           while(*cursor != NULL && ok == 1){
               i = 0;
                while((*cursor)->ch == old[i] && i <= (strlen(old_word) - 1) ){
                    (*cursor) = (*cursor)->next;
                    i++;
                }
                if(i  == strlen(old_word)) 
                ok = 0;
                if(i == 0)
                (*cursor) = (*cursor)->next;
            }
            (*cursor) = (*cursor)->prev;
            //daca s-a gasit sirul old_word
            if(ok == 0){
                for(j = i-1; j >= 0 ; j--){
                    (*cursor)->ch = new[j];
                    (*cursor) = (*cursor)->prev;
                }
                while(strlen(old_word) < strlen(new_word)){
                    node_t *new_node = (node_t*)malloc(sizeof(node_t));
                    new_node->ch = new[j];
                    new_node->next = (*cursor)->next;
                    (*cursor)->next->prev = new_node;
                    (*cursor)->next = new_node;
                    new_node->prev = *cursor;
                    *cursor = (*cursor)->next;
                }

            }
}
//functie pentru stergerea unui sir din text
void dw_word(node_t **cursor, char sir[]) {
    char *token, delim[2] = " \n", *word;
    int ok = 1, i, j;
    token = strtok(sir,delim);
    token = strtok(NULL, delim);
    word = token;

    while(*cursor != NULL && ok == 1){
          i = 0;
          while((*cursor)->ch == word[i] && i <= (strlen(word) - 1) ) {
              (*cursor) = (*cursor)->next;
               i++;
            }
            if(i  == strlen(word)) 
            ok = 0;
            if(i == 0)
            (*cursor) = (*cursor)->next;
        
        }
        (*cursor) = (*cursor)->prev;
          if(ok == 0){
              for(j = i - 1; j >= 0 ; j--) {
                  remove_node(cursor);
                  (*cursor) = (*cursor)->prev;
               
                }
            }
}
void undo(dequeue_t **stiva_undo,dequeue_t **stiva_redo, node_t **cursor,
          int lungime_text) {
          dequeue_t *new_node = (dequeue_t*)malloc(sizeof(dequeue_t));
          dequeue_t *delete;

         while(strcmp((*stiva_undo)->data, "u\n") != 0) {
            delete = *stiva_undo;
            (*stiva_undo) = (*stiva_undo)->next;
            free(delete);
            }
            delete = *stiva_undo;
            *stiva_undo = (*stiva_undo)->next;
            free(delete);

            strcpy(new_node->data,(*stiva_undo)->data);
            *stiva_redo = push_deq(stiva_redo,new_node);
            (*cursor) = (*cursor)->next;

            if(strcmp((*stiva_undo)->data, "::i\n") == 0) {
                for(int i = 0; i < lungime_text ; i++) {
                    if((*cursor)->next != NULL)
                    remove_node(cursor);
                }
            }
}
void redo(dequeue_t **stiva_undo, dequeue_t **stiva_redo, node_t **cursor,
          node_t **text_com, node_t **text) {
              node_t *cursorul = *cursor;
              node_t *c_text = *text;
    
              if(strcmp((*text_com)->prev->data,"u\n") == 0)
              if(strcmp((*stiva_redo)->data,"::i\n") == 0){
                  while(c_text != NULL) {
                      node_t *new_node = (node_t*)malloc(sizeof(node_t));
                      new_node->ch = c_text->ch; 
                      c_text = c_text->next;
                      push_back(&cursorul,new_node);
                   }    
                } 
                if((*cursor)->ch ==  '\n')
                *cursor =  (*cursor)->next;
}
int main(){
    FILE *fp;
    fp = fopen("editor.in", "r");
    FILE* fo = fopen("editor.out", "w");
    char s[50], c;
    int nr_inserari_text = 0, nr_salvari = 0, lungime_text = 0;
   
    node_t *head_comenzi, *head, *text, *cursor;
    dequeue_t *stiva_undo, *stiva_redo;
    
    node_t *delete;
    dequeue_t *sters;

    dequeue_t *nod_inserare = (dequeue_t*)malloc(sizeof(dequeue_t));
    strcpy(nod_inserare->data, "::i\n");

    init(&head);
    init(&cursor);
    alloc_deq(&stiva_undo);
    alloc_deq(&stiva_redo);
    *cursor = *head;

    while(!feof(fp)) {
        nr_inserari_text++;
        lungime_text = 0;
        init(&head_comenzi);
        init(&text);
        if(nr_inserari_text > 1)
        text->ch = '\0';

        while( (c = fgetc(fp)) != ':') {
            node_t *new_node1 = (node_t*)malloc(sizeof(node_t));
            node_t *new_node2 = (node_t*)malloc(sizeof(node_t));
            
            new_node1->ch = c; 
            new_node2->ch = c; 
           
            if(nr_inserari_text == 1)
            push_back(&head,new_node1);
            else push_back(&cursor,new_node1);
                  
            push_back(&text,new_node2);
            lungime_text++;
            }
            while(text->prev != NULL)
            text = text->prev;
    
            fgetc(fp);
            fgetc(fp);
            fgetc(fp);
            stiva_undo = push_deq(&stiva_undo,nod_inserare);
         
            while(fgets(s,50,fp) && s[0] != ':' && s[1]!= ':'){
                dequeue_t *stiva_new_node = (dequeue_t*)malloc(sizeof(dequeue_t));
                node_t *new_node = (node_t*)malloc(sizeof(node_t));
            
                strcpy(stiva_new_node->data,s);
                strcpy(new_node->data,s);
            
                push_back(&head_comenzi,new_node);

                if(strcmp(stiva_new_node->data,"q\n") != 0 && 
                   strcmp(stiva_new_node->data,"s\n"))
                   stiva_undo = push_deq(&stiva_undo,stiva_new_node);
                   else free(stiva_new_node);
                }
                node_t *copie_head_comenzi = head_comenzi;
                copie_head_comenzi = copie_head_comenzi->next;

                while(copie_head_comenzi != NULL) {
                    node_t *copie_cursor = cursor;
                    char sir[50], *token;
                    char str[2] = " ";
            
                    strcpy(sir, copie_head_comenzi->data);
                    token = strtok(sir,str);
        
                    if(strcmp(sir, "s\n") == 0) { 
                        save(&head,&cursor, fo);
                        nr_salvari++;
                        }

                     if(strcmp(sir, "gl") == 0)
                     cursor = gl_line(&head,copie_head_comenzi->data );
            
                     if(strcmp(sir, "dl\n") == 0 || strcmp(token, "dl") == 0)
                     dl_line(&cursor, &head, copie_head_comenzi->data);
       
                     if(strcmp(sir,"gc") == 0)
                     gc_char_line(&cursor, &head, copie_head_comenzi->data);

                     if(strcmp(sir, "b\n") == 0)
                     backspace(&cursor);

                     if(strcmp(sir,"d\n") == 0 || strcmp(sir,"d") == 0)
                     d_chars(&cursor, &head, copie_head_comenzi->data);
        
                     if(strcmp(sir,"re") == 0)
                     re_old_new(&copie_cursor,copie_head_comenzi->data);
        
                     if(strcmp(sir,"dw") == 0)
                     dw_word(&copie_cursor, copie_head_comenzi->data);

                     if(strcmp(sir,"u\n") == 0)
                     undo(&stiva_undo,&stiva_redo, &cursor, lungime_text);
        
                     if(strcmp(sir,"r\n") == 0)
                     redo(&stiva_undo, &stiva_redo,&cursor,
                      &copie_head_comenzi, &text);
         
                     copie_head_comenzi = copie_head_comenzi->next;
                }
                while(head_comenzi != NULL){
                delete = head_comenzi;
                head_comenzi = head_comenzi->next;
                free(delete);
                } 
                while(text != NULL){
                delete = text;
                text = text->next;
                free(delete);
                }
     }
     while(text != NULL){
        delete = text;
        text = text->next;
        free(delete);
        }  
     while(head != NULL && head != cursor){
        delete = head;
        //print_ch(head);
        head = head->next;
        free(delete);
        }        
     while(cursor != NULL){
        //print_ch(cursor);
        delete = cursor;
        cursor = cursor->next;
        free(delete);
        }      

     while(stiva_undo != NULL && stiva_undo != nod_inserare){
        sters = stiva_undo;
        stiva_undo = stiva_undo->next;
        free(sters);
        } 
        free(nod_inserare);
    
       fclose(fp);
       return 0;
}