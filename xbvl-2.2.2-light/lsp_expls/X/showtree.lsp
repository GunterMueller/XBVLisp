;;
; represente une liste comme un arbre dans un widget awTree
; et donne acces a chaque element de la liste a partier de
; cette representation graphique
;;
(de showTree(-list-)
    (if (iswidget 'tree-demo "ApplicationShell") ()
      (xCreateWidget 'tree-demo "ApplicationShell" "x" 0 "y" -1 )
      (xCreateWidget "tree-viewport" "awViewport" 'tree-demo
		     "allowVert"  "True"
		     "allowHoriz" "True"
		     "width" 300
		     "height" 300
		     "useRight"    "True"  	    ))
    (if (iswidget 'tree "awTree")(xRemoveWidget 'tree))
    (let (-tree-  (xCreateWidget "tree" "awTree" tree-viewport))
      (if (listp -list-) 
	  (creeNewBranche -tree- nil -list-)
	(creeTreeList -tree- nil -list-))
      (xRealize 'tree-demo)
      ))
;;
					;
;;
(de creeNewBranche (-treew- -superieur- -list-)
    (let ( -newNode- ( xCreateTreeNode " " -treew- -superieur- -list- ))
      (creeTreeList -treew- -newNode- -list-)
      -newNode-))

					;
;;

(de creeTreeList (-treew- -superieur- -list- )
    (cond
     ((null -list-) t)
     ((listp (car -list-)) 
      (creeNewBranche -treew- -superieur- (car -list-))
      (creeTreeList -treew- -superieur- (cdr -list-)))
     (t (xCreateTreeNode (car -list- ) -treew- -superieur- (car -list-))
	(creeTreeList -treew- -superieur- (cdr -list-) ))))))


;; 
; creation d'un node 
; un callback avec l'objet LISP comme argument est
; enregistre dans chaque nouveau node
;;

(de xCreateTreeNode ( ele -parent- -superieur- objet )
    (let (-treeNode-
	  (cond
	   ((eq 0 (strcmp " " ele))
	    (xCreateWidget "node" "awCommand" -parent-
			   "bitmap" "node.bit"
			   "treeParent" -superieur- ))
	   ((null ele)
	    (xCreateWidget "node" "awCommand" -parent-
			   "treeParent" -superieur- ))
	   (t (let ( name (if(numbp ele) (strcat "n-" ele) ele))
		(xCreateWidget name "awCommand" -parent-
			     ;  "font" "Fixed" ;
			       "label" 
			       (if (stringp ele)
				   (strcat "\"" ele "\"") (strcat ele))
			       "treeParent" -superieur- )))))
      ; l'objet represente ;
      (let (newAtome (gensym))
	(let (atomPn (strcat newAtome))
	  (set newAtome objet)
	  (xAddCallback -treeNode- "callback" atomPn)))
      -treeNode-))


