(import (srfi 25)
        (chicken random)
        (chicken format))

(define (gol X Y I N)
  "init arrays"
  (let ((uni (make-array (shape 0 X 0 Y) #t))
        (cpy (make-array (shape 0 X 0 Y) #t)))
    (do ((i 0 (let ((x (pseudo-random-integer X))
                    (y (pseudo-random-integer Y)))
                (if (array-ref uni x y)
                    (begin
                      (array-set! uni x y #f)
                      (array-set! cpy x y #f)
                      (add1 i))
                    i))))
      ((= i I)))
    "run the game of life"
    (do ((n 0 (add1 n))) ((= n N))
      (printf "P1\n~a ~a\n" Y X)
      (do ((x 0 (add1 x))) ((= x X))
        (do ((y 0 (add1 y))) ((= y Y))
          (printf "~a" (if (array-ref uni x y) 0 1))
          "count neighbors"
          (set! cnt 0)
          (do ((x_o -1 (add1 x_o))) ((= x_o 2))
            (do ((y_o -1 (add1 y_o))) ((= y_o 2))
              (unless (and (zero? x_o)
                           (zero? y_o))
                (unless (array-ref uni
                                   (remainder (+ x x_o X) X)
                                   (remainder (+ y y_o Y) Y))
                  (set! cnt (add1 cnt))))))
          (case cnt
            ((2))
            ((3)  (array-set! cpy x y #f))
            (else (array-set! cpy x y #t)))))
      (do ((x 0 (add1 x))) ((= x X))
        (do ((y 0 (add1 y))) ((= y Y))
          (array-set! uni x y (array-ref cpy x y)))))))

(gol 75 75 2500 500)
