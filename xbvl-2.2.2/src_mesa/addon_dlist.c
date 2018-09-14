/* Ce fichier doit être ajouté à la fin de src/dlist.c de la librairie MESA : 
   il contient les routines nécessaire à xbvl afin de récupérer les données dont
   il a besoin pour les routines supplémentaire de gestion des listes

   DATA_TYPE sera remplacé par le type correspondant à l'architecture (32 ou 64 bits)
*/

/* Récupération des données : 
   0 = taille d'un noeud
   1 = taille de la liste des opcode
   2 = tailles des données pour chaque opcode
   3 = récupération des données d'une liste 
   4 = annonce de la création d'une liste
   5 = annonce de la fin de la création d'une liste
   6 = allocation d'une nouvelle instruction dans une liste
*/
DATA_TYPE mesa_xbvl_link(char data, GLuint value)
{
  GLcontext *ctx;
#ifdef THREADS
    ctx = gl_get_thread_context();
#else
    ctx = CC;
#endif
  switch(data) {
  case 0: return (DATA_TYPE) sizeof(union node); 
  case 1: return (DATA_TYPE) OPCODE_END_OF_LIST; /* récupération du dernier OPCODE */
  case 11: return (DATA_TYPE) OPCODE_CALL_LIST;
  case 12: return (DATA_TYPE) OPCODE_CALL_LIST_OFFSET;
  case 2: return (DATA_TYPE) &InstSize; /* récupération de la taille des données */
  case 3: return (DATA_TYPE) HashLookup(ctx->Shared->DisplayList, value);
  case 31: return (DATA_TYPE) HashLookup(ctx->Shared->DisplayList, ctx->List.ListBase+value);
  case 4: gl_NewList(ctx, value, GL_COMPILE); return (DATA_TYPE) 1;
  case 5: gl_EndList(ctx); return (DATA_TYPE) 1;
  case 6: return (DATA_TYPE) alloc_instruction(ctx, (OpCode) value, InstSize[value]-1); 
  default: return 0;
  }
}

