((label myfunc (lambda (x) (cond
                     ((atom x) x)
                     ((quote t) (myfunc (car x)))))) (quote ((7 2) 3)) )

((label myfunc (lambda (x) (cond
                     ((atom x) x)
                     ('t (myfunc (car x)))))) '((7 2) 3) )