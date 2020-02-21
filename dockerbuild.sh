#!/bin/bash
docker run -v $PWD:/root/Documents -w "/root/Documents" --rm burtonyaboy/kernelbuild:base make $1
