(cond ((eq (quote a) (quote b)) (quote first))
        ((quote t)  (quote second)))

(cond ((eq (quote a) (quote b)) (quote first))
        ((atom (quote (a b)))  (quote second))
        ((quote t)  (quote third)))

(cond ((eq 'a 'b) 'first)
        ('t  'second))

(cond ((eq 'a 'b) 'first)
        ((atom '(a b))  'second)
        ('t  'third))