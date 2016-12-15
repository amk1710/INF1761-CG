/*
* File:   Object.h
* Author: Renan da Fonte Simas dos Santos
*Alterado por Andr� Mazal Krauss
*
* Description: Classe respons�vel por ler arquivos rt5.
*
* Created on December 11, 2016
*/

#pragma once
#ifndef _RT5READER
#define _RT5READER
#include <vector>
#include "RayTracing.h"

int readFile(std::string filename, RayTracing *RTpointer);


std::string readLine(std::ifstream& stream);


#endif