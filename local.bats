#!/usr/bin/env bats

load test_helper

@test "argument vide" {
	
	run ./penv 

	check 2 "penv [-p] PID NOM"
}

@test "argument 1 vide sans p" {
	
	run ./penv 2
	
	check 2 "penv [-p] PID NOM"
}

@test "argument vide avec p" {
	
	run ./penv -p
	
	check 2 "penv [-p] PID NOM"
}

@test "argument 1 vide avec p" {
	
	run ./penv -p 2
	
	check 2 "penv [-p] PID NOM"
}

@test "argument non numerique" {
	
	run ./penv afaf gafa
	
	check 2 "Le premier argument n'est pas un chiffre. Veuillez entrez un PID valide."
}

@test "HELLO WORLD" {
	env HELLO=WORLD sleep 10 &

	run ./penv $! HELLO

	check 0 "WORLD"

	kill $!
}

@test "NEXISTEPAS" {
	sleep 10 &

	run ./penv $! NEXISTEPAS

	check 1 ""

	kill $!
}

@test "chaine vide" {
	env BYE= sleep 10 &

	run ./penv $! BYE

	check 0 ""

	kill $!
}

@test "avec =" {
	env BONJOUR=LE=MONDE sleep 10 &

	psleep=`pgrep -s 0 sleep`

	run ./penv $! BONJOUR

	check 0 "LE=MONDE"

	kill $!
}








    


