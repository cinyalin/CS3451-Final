set -e

if [ -z "$1" ]
then
	echo >&2 "No assignment given.";
	exit 1;
fi

assignment=$1

echo "Building project $assignment"

cmake --build ./build --config Release --target $assignment

cd ./tutorials/$assignment
./../../build/tutorials/$assignment/$assignment

cd ../..