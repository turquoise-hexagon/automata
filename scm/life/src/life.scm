(import (chicken bitwise)
        (chicken random)
        (chicken format)
        (chicken pathname)
        (chicken process-context))

; helper functions
(define (errx msg)
  (format (current-error-port) msg)
  (exit 1))

(define (usage)
  (errx (format "\
usage : ~a [-whi <number>]

options :
    -w    set width  to <number> (default : 150)
    -h    set height to <number> (default : 150)
    -i    iterate <number> times (default : 500)"
    (pathname-file (program-name)))))

(define (convert-to-number str)
  (let ((result (string->number str)))
    (if (and result
             (> result 0))
        result
        (errx (format "error : '~a' isn't a valid positive integer" str)))))

; main functions
(define (life height width iter)
  ; init array
  (let ((uni (make-vector height)))
    (do ((i 0 (add1 i))) ((= i height))
      (vector-set! uni i (make-vector width))
      (do ((j 0 (add1 j))) ((= j width))
        (vector-set! (vector-ref uni i) j (make-vector 2 0))))
    (do ((i 0 (add1 i))) ((= i (/ (* height height) 4)))
      (vector-set! (vector-ref (vector-ref uni (pseudo-random-integer height))
                                               (pseudo-random-integer width))
                                               0 1))
    ; run the game of life
    (set! flag 0)
    (do ((n 0 (add1 n))) ((= n iter))
      (printf "P1\n~a ~a\n" width height)
      (set! neg (bitwise-xor flag 1))
      (do ((i 0 (add1 i))) ((= i height))
        (do ((j 0 (add1 j))) ((= j width))
          (printf "~a" (vector-ref (vector-ref (vector-ref uni i) j) flag))
          ; count neighbors
          (set! cnt 0)
          (do ((k -1 (add1 k))) ((> k 1))
            (do ((l -1 (add1 l))) ((> l 1))
              (unless (and (zero? k)
                           (zero? l))
                (set! cnt (+ cnt (vector-ref (vector-ref (vector-ref uni (remainder (+ i k height) height))
                                                                         (remainder (+ j l width) width))
                                                                         flag))))))
          (case cnt
            ((2)  (set! tmp (vector-ref (vector-ref (vector-ref uni i) j) flag)))
            ((3)  (set! tmp 1))
            (else (set! tmp 0)))
          (vector-set! (vector-ref (vector-ref uni i) j) neg tmp)))
      (set! flag neg))))

; argument parsing
(let ((width  150)
      (height 150)
      (iter   500))
  (let helper ((lst (command-line-arguments)))
    (unless (null? lst)
      (cond ((and (string=? (car lst) "-w")
                  (not (null? (cdr lst))))
             (set! width  (convert-to-number (cadr lst)))
             (helper (cddr lst)))
            ((and (string=? (car lst) "-h")
                  (not (null? (cdr lst))))
             (set! height (convert-to-number (cadr lst)))
             (helper (cddr lst)))
            ((and (string=? (car lst) "-i")
                  (not (null? (cdr lst))))
             (set! iter   (convert-to-number (cadr lst)))
             (helper (cddr lst)))
            ((string=? (car lst) "--")
             (helper (list)))
            (else (usage)))))
  (life height width iter))
