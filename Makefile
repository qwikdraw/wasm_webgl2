# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: logan  <logan@42.us.org>                   +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2018/03/13 10:03:24 by logan             #+#    #+#              #
#    Updated: 2018/09/03 20:58:31 by twalton          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = index
LIST = main \
GLWindow \
Input \
ShadingProgram \
Rectangle \
Texture

SRC_DIR = src
OBJ_DIR = obj

SRC = $(addsuffix .cpp, $(addprefix src/, $(LIST)))
OBJ = $(addsuffix .bc, $(addprefix $(OBJ_DIR)/, $(LIST)))
DEP = $(OBJ:%.bc=%.d)
CC = em++
SUB =
INCLUDES=-I ~/.brew/include

CPPFLAGS = -Wall -Wextra -Werror \
-O3 -s USE_WEBGL2=1 -s ALLOW_MEMORY_GROWTH=1 -std=c++14 -s WASM=1 \
$(INCLUDES)

#LDFLAGS = --llvm-lto 3 -O3 --closure 1 --preload-file assets
LDFLAGS = --preload-file assets

all: $(OBJ_DIR) $(NAME)

$(NAME): $(OBJ)
	@for s in $(SUB);\
	do\
		make -sC $$s;\
	done
	@printf "\e[32;1mLinking.. \e[0m\n"
	@$(CC) $(LDFLAGS) $^ -o $@.js
	@printf "\e[32;1mCreated:\e[0m %s\n" $(NAME)

$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)

-include $(DEP)

$(OBJ_DIR)/%.bc: $(SRC_DIR)/%.cpp
	@printf "\e[34;1mCompiling: \e[0m%s\n" $<
	@$(CC) $(CPPFLAGS) -MMD -c $< -o $@

clean:
	@for s in $(SUB);\
	do\
		make -sC $$s clean;\
	done
	@printf "\e[31;1mCleaning..\e[0m\n"
	@rm -f $(OBJ) $(DEP)

fclean:
	@for s in $(SUB);\
	do\
		make -sC $$s fclean;\
	done
	@printf "\e[31;1mFull Cleaning..\e[0m\n"
	@rm -rf $(OBJ_DIR)
	@rm -f $(NAME).js
	@rm -f $(NAME).wasm
	@rm -f $(NAME).data

run: all
	python3 -m http.server 8080

re:	fclean all

.PHONY: clean fclean all re
