#!/bin/bash
docker run -v $PWD:/root/Documents -w "/root/Documents" --rm buildkernel make $1
