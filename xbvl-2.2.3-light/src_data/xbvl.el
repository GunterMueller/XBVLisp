;;; xbvl.el --- Major mode Xbvl (Bois de Vincennes Lisp)

;; Copyright (C) 2000 by Valvassori Moïse

;; Author: Valvassori Moïse <djedi@altern.org>
;; Keywords: lisp, languages

;; This file is free software; you can redistribute it and/or modify
;; it under the terms of the GNU General Public License as published by
;; the Free Software Foundation; either version 2, or (at your option)
;; any later version.

;; This file is distributed in the hope that it will be useful,
;; but WITHOUT ANY WARRANTY; without even the implied warranty of
;; MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
;; GNU General Public License for more details.

;; You should have received a copy of the GNU General Public License
;; along with GNU Emacs; see the file COPYING.  If not, write to
;; the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
;; Boston, MA 02111-1307, USA.

;;; Commentary:

;; Installation:
;; Add the following lines in your .emacs.el
;; (autoload 'xbvl-mode "/usr/local/share/emacs/site-lisp/xbvl" "Major mode for editing xbvl code." t)
;; (setq auto-mode-alist (cons '("\.l$" . xbvl-mode)
;;			    auto-mode-alist))


;;; Code:
(provide 'xbvl)

(defconst xbvl-font-lock-keywords
  (eval-when-compile (list
;   '("^\( *\\(de\\) +\\(\\sw+\\) +\( *\\(.*\\) *\)" 
; ";[^;]*\\($\\|;\\)" commentaire
   (list (concat "^\( *"
		 (regexp-opt
		  '("de" "df" "dm" "defmacro") t)
		 " +\\(\\sw.+\\) +\( *\\(.*\\) *\)") 
	 '(1 font-lock-keyword-face)
	 '(3 font-lock-function-name-face)
	 '(4 font-lock-variable-name-face))
   
   '("^\( *\\(setq\\) +\\(\\sw+\\)" 
     (1 font-lock-keyword-face)
     (2 font-lock-variable-name-face))

    (list (concat "( *" (regexp-opt 
	   '("eval" "apply" "time" "oblist" "gc" "strop" "sh" "ls" "cwd" "session" "argc" "argv" "user" "tty" "getenv" "status" "aide" "sections" "listesection"
	     "quote" "progn" "eprogn" "prog1" "if" "ifn" "when" "unless" "cond" "selectq" "repeat" "while" "until" "do"
	     "de" "df" "pretty" "trace" "untrace" "lambda" "let" "self" "letdic" "letf" "closure" "exit" "escape" "lock" "unwind-protect" "unwind"
	     "dm" "defmacro" "macroexpand" "backquote" "quasiquote" "diese" "dmc" "dmo"
	     "atom" "listp" "numbp" "litatom" "stringp" "null" "eq" "=" "equal" "neq" ">" "ge" "<" "le" "zerop" "or" "and" "not"
	     "car" "cdr" "cons" "caar" "cadr" "cdar" "cddr" "caaar" "caadr" "cadar" "caddr" "cdaar" "cdadr" "cddar" "cdddr" "nth" "n" "list" "mcons" "append" "length" "last" "evlis" "reverse" "delete" "delq" "member" "memq" "vmemq" "mmemq" "copy" "copy-all" "subst" "mapc" "mapct" "p-listes" "get" "put" "addprop" "remprop" "a-listes" "assoc" "assq" "pairlis" "sublis"
	     "atomes" "gensym" "explode" "implode" "atoi" "itoa" "remob" "fval" "ftyp"
	     "flottants" "+" "-" "*" "/" "rem" "1+" "1-" "ibase" "obase" "random" "lognot" "logand" "logor" "logxor" "logshift" "fix" "float" "sqrt" "sin" "cos" "asin" "acos" "atan"
	     "dupl" "strcmp" "strcat" "strcar" "strcdr" "plength" "strincp"
	     "setq" "set" "deset" "rplaca" "rplacb" "rplac" "nextl" "newl" "incr" "decr" "nconc" "nreverse" ":=" "attach"
	     "print" "princ" "prin1" "terpri" "princh" "read" "readch" "peekch" "readline" "lib" "include" "input" "output" "open" "fread" "close" "tyo" "tyi" "typch" "outpos" "probef" "rtext"
	     "editeur" "page-editor" "edit" "crossfile" "prettyfile" "crossf" "prettyf" "user-doc" "ind" 
	     "background" "wait" "break" "date" "time" "undmc" "redmc" "issuser" "getpackage" "package" "unstep" "step" "stepr" "stp"
	     "undo" "done" "first-call" "chemin-entree" "chemin-sortie" "chemin" "changeval" "getcar" "getcr" "putcar" "putcdr" "clearcdr" "clearcar" "careful" "example" "erreur" "entree" "sortie" "enleve" "enleve-sortie" "assert-entree" "assert-sortie" "unassert" "reassert" "test-entree" "test-sortie" "ival" "clrival" "globalvar" "localvar" "all-functions" "sortie" "ell" "not-all" 
	     "nomacout" "macout" "boundp" "readlin" "dml" "extprin1" "allprint" "allprin1" "getdoc" "allpretty"
	     ) t)
		  "\\>")
	  '(1 font-lock-builtin-face))

    (list (concat "\\<" (regexp-opt 
	   '("toplevel" "prompt" "it" "printlength" "EOF" "t" "nil") t)
		  "\\>")
	  '(1 font-lock-variable-name-face))
    )))


;;;###autoload
(define-derived-mode xbvl-mode
  lisp-mode "Xbvl"
  "Major mode for XBVL."
  (make-local-variable 'font-lock-defaults)
  (setq font-lock-defaults '(xbvl-font-lock-keywords nil))
  (run-hooks 'xbvl-mode-hook)
  (message "Bienvenue dans Xbvl")
  )
;;; xbvl.el ends here