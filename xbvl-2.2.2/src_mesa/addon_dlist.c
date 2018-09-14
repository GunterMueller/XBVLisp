/* Ce fichier doit �tre ajout� � la fin de src/dlist.c de la librairie MESA : 
   il contient les routines n�cessaire � xbvl afin de r�cup�rer les donn�es dont
   il a besoin pour les routines suppl�mentaire de gestion des listes

   DATA_TYPE sera remplac� par le type correspondant � l'architecture (32 ou 64 bits)
*/

/* R�cup�ration des donn�es : 
   0 = taille d'un noeud
   1 = taille de la liste des opcode
   2 = tailles des donn�es pour chaque opcode
   3 = r�cup�ration des donn�es d'une liste 
   4 = annonce de la cr�ation d'une liste
   5 = annonce de la fin de la cr�ation d'une liste
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
  case 1: return (DATA_TYPE) OPCODE_END_OF_LIST; /* r�cup�ration du dernier OPCODE */
  case 11: return (DATA_TYPE) OPCODE_CALL_LIST;
  case 12: return (DATA_TYPE) OPCODE_CALL_LIST_OFFSET;
  case 2: return (DATA_TYPE) &InstSize; /* r�cup�ration de la taille des donn�es */
  case 3: return (DATA_TYPE) HashLookup(ctx->Shared->DisplayList, value);
  case 31: return (DATA_TYPE) HashLookup(ctx->Shared->DisplayList, ctx->List.ListBase+value);
  case 4: gl_NewList(ctx, value, GL_COMPILE); return (DATA_TYPE) 1;
  case 5: gl_EndList(ctx); return (DATA_TYPE) 1;
  case 6: return (DATA_TYPE) alloc_instruction(ctx, (OpCode) value, InstSize[value]-1); 
  default: return 0;
  }
}

