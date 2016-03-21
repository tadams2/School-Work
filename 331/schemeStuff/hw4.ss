;;; cmsc 331 w4, spring 2011.  ted adams, tadams2@umbc.edu

(define (pass)
  ;; this is a a dummy function.  you should replace calls to it with
  ;; your own scheme code
  null)


(define (countzero ls)
  ;;  returns the number of zeros in a given simple list of                                                                                
  ;;  numbers. The input will be a (possibly empty) list of                                                                                
  ;;  integers. For example, (countZeros '(0 1 2)) should return the                                                                       
  ;;  value 1.
  (if (null? ls)0
      (if(= (car ls) 0)(+ (countZero (cdr ls)) 1)(countZero (cdr ls))
      )
  )
)

;;helper functions I defined to help findMinMax
(define (minNumb ls)
  (minNum ls (car ls))
)

(define (minNum ls minVal)
  (cond((null? ls) minVal)

       (( < (car ls) minVal)
        (minNum (cdr ls) (car ls)))

       (else
        (minNum (cdr ls) minVal))
  )
)


(define (maxNumb ls)
  (maxNum ls (car ls))
)

(define (maxNum ls maxVal)
  (cond((null? ls) maxVal)

       (( > (car ls) maxVal)
        (maxNum (cdr ls) (car ls)))

       (else
        (maxNum (cdr ls) maxVal))
  )
)


(define (findMinMax ls)
  ;; returns a list consisting of the smallest and largest values in
  ;; the list. For example, (findMinMax '(0 1 2)) should return the
  ;; list (0 2)
     (list (minNumb ls) (maxNumb ls))
)


(define (chopList aList) 
  ;; takes a list and removes the last element of the list. For
  ;; example, (chopList '(0 1 2)) should return the list (0 1). Make
  ;; sure that (chopList '()) returns the empty list
  (if(null? aList) null
     (reverse(cdr(reverse aList)))
     )
)


  

(define (unravel aList)
  ;; takes a list with possible many nested sublists, and returns a
  ;; list with all atoms at the top level. For example, 
  ;; (unravel '(1 (2 3) (a (b c)))) should return the list (1 2 3 a b c)
  (cond
   ((null? aList)null)
   ((list? aList)(append (unravel (car aList))(unravel (cdr aList))))
    (else (list aList))))


(define (atom? x) (not (or(pair? x)(null? x))))

(define (eqStruc l1 l2)
  ;; tests for the structural equality of two input lists. Two lists
  ;; are structurally equal if they have the same list structure,
  ;; although their atoms may be different. For example, the lists '(1
  ;; 2 3 ) and '(4 5 6) are structurally equal. The lists '(a (b c) d)
  ;; and '(a b (c d)) are not structurally equal.
  (cond
   ((and (null? l1) (null? l2))
    #t)
   ((or (and (null? l1) (null? l2)) (and (null? l2) (null? l1)))
    #f)
   ((and (atom? (car l1)) (atom? (car l2))) 
    (eqStruc (cdr l1) (cdr l2)))
   ((or (and (atom? (car l1)) (not (atom? (car l2)))) (and (atom? (car l2)) (not (atom? (car l1)))))
    #f)
   (else (and (eqStruc (car l1) (car l2)) (eqStruc (cdr l1) (cdr l2))))
   )
  )


