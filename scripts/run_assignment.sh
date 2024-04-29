set -e

if [ -z "$1" ]
then
	echo >&2 "No assignment given.";
	exit 1;
fi

assignment=$1
assignment_name="${assignment}"

echo "Building project $assignment"

if [ "$assignment" != "tech_proj" ] && [ "$assignment" != "dirt_proj" ]
then
	cmake --build ./build --config Release --target $assignment_name

	cd ./assignments/$assignment
	./../../build/assignments/$assignment/$assignment_name
else
	cmake --build ./build --config Release --target $assignment_name

	cd ./assignments/$assignment
	./../../build/assignments/$assignment/$assignment_name
fi

cd ../..