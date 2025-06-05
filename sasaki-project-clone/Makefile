scons:
	python3 -m venv ~/venv
	source ~/venv/bin/activate
	pip install scons

fresh:
	rm -rf godot-cpp
	git clone -b 4.3 https://github.com/godotengine/godot-cpp/ temp-godot-cpp
	mkdir godot-cpp
	mv temp-godot-cpp/* ./godot-cpp/
	rm -rf temp-godot-cpp
	git submodule update  --init --recursive
	cd godot-cpp/gdextension && godot --dump-extension-api
	cd godot-cpp && scons
	scons

.PHONY: clean
changes:
	scons
