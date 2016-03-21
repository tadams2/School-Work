
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


(define (chopList ls)
  (reverse(cdr(reverse ls)))
)


(define (eqStruc l1 l2)
(cond 
((and (null? l1) (null? l2)) #t)
((or (and (null? l1) (null? l2)) (and (null? l2) (null? l1))) #f)
((and (atom? (car l1)) (atom? (car l2))) (eqStruc (cdr l1) (cdr l2)))
((or (and (atom? (car l1)) (not (atom? (car l2)))) (and (atom? (car l2)) (not (atom? (car l1))))) #f)
(else (and (eqStruc (car l1) (car l2)) (eqStruc (cdr l1) (cdr l2))))
)
)

(define (atom? x) (not (or (pair? x) (null? x))))