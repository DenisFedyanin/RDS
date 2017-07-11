(defn cart [colls]
  (if (empty? colls)
    '(())
    (for [x (first colls)
          more (cart (rest colls))]
      (concat x more)
    )
  )
)
			
(defn dicho-sol 
	[node p w]
	(cond
		(not (.contains (node :data) [p w])) 
			nil
		(= (node :left) (node :right) nil) 
			(list (list [p w]))
		(and (node :left) (node :right) (not (= (node :left) (node :right))))
			(concat
				(apply concat
					(for [x ((node :left) :data)
								y ((node :right) :data)]
						(if (and (= p (+ (x 0) (y 0))) (= w (+ (x 1) (y 1)))) 
							(let [dl (dicho-sol (node :left) (x 0) (x 1))
								dr (dicho-sol (node :right) (y 0) (y 1))]
								;(println (str "in if" x y dl dr))
								(cart (list dl dr))
							)
						)
					)
				)
				;(let [dl (dicho-sol (node :left) p w)] (comment (println "dl pw" [p w] dl)) dl)
				;(let [dr (dicho-sol (node :right) p w)] (comment (println "dr pw" [p w] dr)) dr)
				(dicho-sol (node :left) p w)
				(dicho-sol (node :right) p w)
			)
		:default (throw (Exception. "strange tree!"))
	)
)

(def mytree (read-string (slurp "period0.cljdata")))
(def pw (last (mytree :data)))
(def s (dicho-sol mytree (pw 0) (pw 1)))

(defn save-decision
	[filename decis]
	(with-open [file (io/writer filename)]
		(.write file (str (count decis) "\n"))
		(doseq [el decis]
			(.write file (str (el 0) " " (el 1) "\n"))
		)
	)
)