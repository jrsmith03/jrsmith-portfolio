# From the CS373 Collatz makefile, with modifications for the specific tools and files used in this project.
config:
	git config -l


# get git status
status:
	@echo
	git branch
	git remote -v
	git status

# upload files to the Website code repo
push:
	make clean
	@echo
	git add *
	git commit -m "default commit"
	git push
	git status

# output versions of all tools
versions:
	uname -p

	@echo
	uname -s

	@echo
	which git
	@echo
	git --version

	@echo
	which make
	@echo
	make --version | head -n 1

	@echo
	which pip
	@echo
	pip --version

	@echo
	which $(Flask)
	@echo
	flask --version

	@echo
	which $(PYTHON)
	@echo
	$(PYTHON) --version

	@echo
	which vim
	@echo
	vim --version | head -n 1

