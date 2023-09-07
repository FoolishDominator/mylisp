((label myfunc (lambda (x) (cond
                     ((atom x) x)
                     ((quote t) (myfunc (car x)))))) (quote ((7 2) 3)) )

                     (myfunc (label myfunc (lambda (x) (cond
                     ((atom x) x)
                     ((quote t) (myfunc (car x)))))))
