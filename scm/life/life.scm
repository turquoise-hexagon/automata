(import (chicken bitwise)
        (chicken format)
        (chicken pathname)
        (chicken process-context)
        (chicken random))

(define (gol X Y I N)
  "init vector"
  (let ((uni (make-vector X)))
    (do ((x 0 (add1 x))) ((= x X))
      (vector-set! uni x (make-vector Y))
      (do ((y 0 (add1 y))) ((= y Y))
        (vector-set! (vector-ref uni x) y (make-vector 2 0))))
    (do ((i 0 (add1 i))) ((= i I))
      (vector-set! (vector-ref (vector-ref uni (pseudo-random-integer X))
                                               (pseudo-random-integer Y))
                                               0 1))
    "run the game of life"
    (set! flag 0)
    (do ((n 0 (add1 n))) ((= n N))
      (printf "P1\n~a ~a\n" Y X)
      (set! neg (bitwise-xor flag 1))
      (do ((x 0 (add1 x))) ((= x X))
        (do ((y 0 (add1 y))) ((= y Y))
          (printf "~a" (vector-ref (vector-ref (vector-ref uni x) y) flag))
          "count neighbors"
          (set! cnt 0)
          (do ((u -1 (add1 u))) ((> u 1))
            (do ((v -1 (add1 v))) ((> v 1))
              (unless (and (zero? u)
                           (zero? v))
                (set! cnt (+ cnt (vector-ref (vector-ref (vector-ref uni (remainder (+ x u X) X))
                                                                         (remainder (+ y v Y) Y))
                                                                         flag))))))
          (case cnt
            ((2)  (set! tmp (vector-ref (vector-ref (vector-ref uni x) y) flag)))
            ((3)  (set! tmp 1))
            (else (set! tmp 0)))
          (vector-set! (vector-ref (vector-ref uni x) y) neg tmp)))
      (set! flag neg))))

"argument parsing"
(let ((lst (command-line-arguments)))
  (if (= (length lst) 4)
      (apply gol
             (map
               (lambda (x)
                 (let ((tmp (string->number x)))
                   (if (or (not tmp)
                           (< tmp 0))
                       (begin
                         (format (current-error-port)
                                 "error : '~a' isn't a valid positive integer\n"
                                 x)
                         (exit 1))
                       tmp)))
               lst))
      (begin
        (format (current-error-port)
                "usage : ~a [height] [width] [init] [iter]\n"
                (pathname-file (program-name)))
        (exit 1))))
