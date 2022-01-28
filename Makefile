all: penv

check: penv
	bats check.bats
