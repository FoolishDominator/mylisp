(quote 23)
(quote a)
(quote (a (b c) (d e f)))
"---------"
(atom (quote 342))
(atom (quote (342)))
(atom (quote ()))
"hello world"
(atom "hello world")
"---------"
(eq (quote abc) (quote abc))
(eq (quote abc) (quote abcd))
(eq (quote ()) (quote ()))
"---------"
(car(cdr(cdr (quote (a (b c d) (e f))))))
(cdr(cdr (quote(a (b c d) (e f)))))
"---------"
(cons (quote a) (cons (quote b) (cons (quote c) (quote()))))
"---------"
(cond ((eq (quote a) (quote b)) (quote first))
        ((quote t)  (quote second)))

(cond ((eq (quote a) (quote b)) (quote first))
        ((atom (quote (a b)))  (quote second))
        ((quote t)  (quote third)))
"---------"
((lambda (x) (cons x (quote (b)) )) (quote a))
((lambda (x y) (cons x (cdr y)))
   (quote z)
   (quote (a b c)) )
"---------"
((label myfunc (lambda (x) (cond
                     ((atom x) x)
                     ((quote t) (myfunc (car x)))))) (quote ((7 2) 3)) )

