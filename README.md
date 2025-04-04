# Model Checking

- CTL model checker
- Input can be specified in the input.txt
- following the syntax
- States - <capital letter><one digit number>
- propositional variables - <[a-z]>

    - beginKripke
        STATES : [<states as comma seperated like S0, S1, S2, S3, S4>]
        TRANSITIONS : 
        [
        	<from_state> : <to_state>,
        	<from_state> : <to_state>,
        	<other transitions.....>
      
        ]
        LABELS : 
        [
        	<state> : [<labels as comma seperated>],
        	<....other states>
        
        ]
        endKripke
        
        beginCTL
       < CTL formula to check >
        endCTL
