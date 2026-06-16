/*
** EPITECH PROJECT, 2026
** argHandler
** File description:
** TextFmt
*/

#pragma once

#define ESC "\033"
#define MY_FORMAT(n) ESC "[" #n "m"

#define RESET MY_FORMAT(0)
#define BOLD MY_FORMAT(1)

#define RED MY_FORMAT(31)
#define GREEN MY_FORMAT(32)
#define YELLOW MY_FORMAT(33)
#define BLUE MY_FORMAT(34)
#define PURPLE MY_FORMAT(35)
#define CYAN MY_FORMAT(36)
