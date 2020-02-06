#!/bin/sh

#mount none /sys/kernel/debug -t debugfs

echo "function_graph" > /sys/kernel/debug/tracing/current_tracer

echo "*:mod:keystroke" > /sys/kernel/debug/tracing/set_ftrace_filter

#echo "*" > /sys/kernel/debug/tracing/set_ftrace_filter

#echo "do_IRQ" > /sys/kernel/debug/tracing/set_ftrace_filter

echo 1 > /sys/kernel/debug/tracing/tracing_on

echo "Tracing started: Press Enter to stop..."
read

echo 0 > /sys/kernel/debug/tracing/tracing_on

echo "nop" > /sys/kernel/debug/tracing/current_tracer 
