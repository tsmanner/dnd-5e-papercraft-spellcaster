#!/usr/bin/env bash
set -e
_ref=$(git branch --show-current)


prs="$(gh pr list --json number,title,baseRefName,headRefName)"


if [ -e deploy ]
then
  rm -rf deploy
fi
mkdir deploy


query() {
  echo $prs | jq --raw-output --jsonargs "$1"
}


checkout() {
  _pr=$1
  git checkout -q -f origin/main
  git merge -m "Merge PR#$_pr to generate page." origin/$(query ".[] | select(.number==$_pr) | .headRefName")
}


gen() {
  _title=$1
  _pr=$2
  printf "$_title($_pr)\n"
  printf -- "- [$_title](./$_pr/index.html)\n" >> build/links.md
  cmake -B build -S src
  cmake --build build
  mkdir deploy/$_pr
  build/main > deploy/$_pr/index.html
  cp templates/styles.css deploy/$_pr/styles.css
}


if ! [ -e build ]
then
  mkdir build
fi

printf "" > build/links.md


git checkout -f origin/main
gen Main main


# For each open PR, render the page in a subdir of the build directory.
for _pr in $(query ".[].number")
do
  checkout $_pr
  gen "$(query ".[] | select(.number==$_pr) | .title")" $_pr
done

printf "" > deploy/index.html
printf "<!DOCTYPE html>\n" >> deploy/index.html
printf "<html>\n" >> deploy/index.html
printf "<head><link rel="stylesheet" href="main/styles.css"/></head>\n" >> deploy/index.html
build/_deps/cmark-gfm-build/src/cmark-gfm build/links.md >> deploy/index.html
printf "</html>\n" >> deploy/index.html


git checkout $_ref
