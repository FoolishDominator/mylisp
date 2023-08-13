((lambda (x) (cons x (quote (b)) )) (quote a))
((lambda (x y) (cons x (cdr y)))
   (quote z)
   (quote (a b c)) )

((lambda (x) (cons x '(b) )) 'a)
((lambda (x y) (cons x (cdr y)))
   'z
   '(a b c) )