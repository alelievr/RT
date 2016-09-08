# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: alelievr <alelievr@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2015/04/04 19:22:36 by alelievr          #+#    #+#              #
#    Updated: 2016/09/08 20:53:27 by alelievr         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

#################
##  VARIABLES  ##
#################

#	Sources
SRCDIR		=	src
SRC			=	./aliase.c\
				./btree_apply_infix.c\
				./btree_apply_prefix.c\
				./btree_apply_suffix.c\
				./btree_create_node.c\
				./error.c\
				./ft_abs.c\
				./ft_aputstr.c\
				./ft_atof.c\
				./ft_atoi.c\
				./ft_atoll.c\
				./ft_bzero.c\
				./ft_convert.c\
				./ft_deconvert.c\
				./ft_factorial.c\
				./ft_firstocc.c\
				./ft_getocc.c\
				./ft_intlen.c\
				./ft_isalnum.c\
				./ft_isalpha.c\
				./ft_isascii.c\
				./ft_isblank.c\
				./ft_isdigit.c\
				./ft_isprint.c\
				./ft_ispunct.c\
				./ft_isspace.c\
				./ft_isupper.c\
				./ft_itoa.c\
				./ft_lastocc.c\
				./ft_lstadd.c\
				./ft_lstdel.c\
				./ft_lstdelone.c\
				./ft_lsteadd.c\
				./ft_lstiter.c\
				./ft_lstmap.c\
				./ft_lstnew.c\
				./ft_lstsplit.c\
				./ft_memalloc.c\
				./ft_memccpy.c\
				./ft_memchr.c\
				./ft_memcmp.c\
				./ft_memcpy.c\
				./ft_memdel.c\
				./ft_memdup.c\
				./ft_memmem.c\
				./ft_memmove.c\
				./ft_memnew.c\
				./ft_memocc.c\
				./ft_memset.c\
				./ft_memswap.c\
				./ft_nbrocc.c\
				./ft_ndeconvert.c\
				./ft_nmatch.c\
				./ft_nnbrocc.c\
				./ft_perror.c\
				./ft_pow.c\
				./ft_print_list.c\
				./ft_ptrswap.c\
				./ft_putascii.c\
				./ft_putchar.c\
				./ft_putchar_color.c\
				./ft_putchar_fd.c\
				./ft_putdouble.c\
				./ft_putendl.c\
				./ft_putendl_fd.c\
				./ft_putlong.c\
				./ft_putlong_fd.c\
				./ft_putnbr.c\
				./ft_putnbr_color.c\
				./ft_putnbr_fd.c\
				./ft_putnbrpos.c\
				./ft_putstr.c\
				./ft_putstr_color.c\
				./ft_putstr_fd.c\
				./ft_putstrcpos.c\
				./ft_putstrcpos_fd.c\
				./ft_putstrpos.c\
				./ft_shl.c\
				./ft_shr.c\
				./ft_sqrt.c\
				./ft_str0cat.c\
				./ft_strcapitalize.c\
				./ft_strcat.c\
				./ft_strchr.c\
				./ft_strclr.c\
				./ft_strcmp.c\
				./ft_strcpy.c\
				./ft_strcut.c\
				./ft_strdel.c\
				./ft_strdup.c\
				./ft_strequ.c\
				./ft_striter.c\
				./ft_striteri.c\
				./ft_strjoin.c\
				./ft_strlcat.c\
				./ft_strlen.c\
				./ft_strlowcase.c\
				./ft_strmap.c\
				./ft_strmapi.c\
				./ft_strmatch.c\
				./ft_strn0cat.c\
				./ft_strncat.c\
				./ft_strnchr.c\
				./ft_strncmp.c\
				./ft_strncpy.c\
				./ft_strndup.c\
				./ft_strnequ.c\
				./ft_strnew.c\
				./ft_strnstr.c\
				./ft_strrchr.c\
				./ft_strrev.c\
				./ft_strsplit.c\
				./ft_strsplitstr.c\
				./ft_strstr.c\
				./ft_strsub.c\
				./ft_strtr.c\
				./ft_strtrim.c\
				./ft_strtrimc.c\
				./ft_strtrstr.c\
				./ft_strupcase.c\
				./ft_swap.c\
				./ft_tolower.c\
				./ft_toupper.c\
				./get_next_line.c\
				./m_error.c\
				./ft_intsize.c\
				./ft_strlcpy.c\
				./rgb.c\
				./ft_exit.c\
				./cmp_float.c\

#	Objects
OBJDIR		=	obj

#	Includes
INCDIR		=	./ libft

#	Libraries
LIBDIR		=	libft
LIB			=	-lft

#	Output
NAME		=	libft.a

#	Compiler
CFLAGS		=	-Werror -Wall -Wextra -g -O2 -funroll-loops -fno-strict-aliasing
CC			=	gcc


################
##   COLORS   ##
################

CLEAN_COLOR		=	9
CLEAN_COLOR_T	=	196
LINK_COLOR		=	47
LINK_COLOR_T	=	46
OBJ_COLOR		=	122
OBJ_COLOR_T		=	93
NORM_COLOR		=	153
NORM_COLOR_T	=	141
NORM_COLOR_ERR	=	160


#################
##    AUTO     ##
#################

SHELL		=	/bin/zsh
OBJ			=	$(addprefix $(OBJDIR)/,$(notdir $(SRC:.c=.o)))
LIBFLAG		=	$(addprefix -L,$(LIBDIR))
INCFLAG		=	$(addprefix -I,$(INCDIR))
NORME		=	$(addsuffix /*.h,$(INCDIR)) $(addprefix $(SRCDIR)/,$(SRC))

$(foreach S,$(SRC), \
	$(eval VPATH_TMP += $(SRCDIR)/$(dir $(S)) \
	))
VPATH		=	$(VPATH_TMP:./=)

disp_indent	=	for I in `seq 1 $(MAKELEVEL)`; do \
				test "$(MAKELEVEL)" '!=' '0' && printf "\t"; \
				done

exec_color	=	$(call disp_indent); \
				echo $(1)$(2) $(3) $(4) $(5) $(6) $(7) $(8) $(9) $(10)"\033[31m"; \
				$(2) $(3) $(4) $(5) $(6) $(7) $(8) $(9) $(10)

disp_title	=	$(call disp_indent); \
				echo "\033[38;5;$(2);m[  $(1) \#$(MAKELEVEL)  ]\033[0m"


#################
##  TARGETS    ##
#################

#	First target
all: $(NAME)

#	Linking
$(NAME): $(OBJ)
	@$(call disp_title,Linking,$(LINK_COLOR_T));
	@$(call exec_color, "\033[38;5;$(LINK_COLOR_T)m➤ \033[38;5;$(LINK_COLOR)m",\
		ar rc, $(NAME), $(OBJ))
#	@ranlib $(NAME)

#	Objects compilation
$(OBJDIR)/%.o: %.c
	@if [ $(ALREADY_OBJ)x != xx ]; then \
		$(call disp_title,Building,$(OBJ_COLOR_T)); \
		fi
	@mkdir $(OBJDIR) 2> /dev/null || echo "" > /dev/null
	@$(call exec_color,"\033[38;5;$(OBJ_COLOR_T)m➤ \033[38;5;$(OBJ_COLOR)m",\
		$(CC), $(CFLAGS), $(INCFLAG), -o, $@, -c, $<) # <- Building command
	@$(eval ALREADY_OBJ=x)

#	Removing objects
clean:
	@if [ $(ALREADY_RM)x != xx ]; then \
		$(call disp_title,Cleaning,$(CLEAN_COLOR_T)); \
		fi
	@$(call exec_color,"\033[38;5;$(CLEAN_COLOR_T)m➤ \033[38;5;$(CLEAN_COLOR)m",\
		rm -f, $(OBJ)) # <- Cleaning objs
	@rmdir $(OBJDIR) 2> /dev/null || echo "" > /dev/null
	@$(eval ALREADY_RM=x)

#	Removing objects and exe
fclean: clean
	@if [ $(ALREADY_RM)x != xx ]; then \
		$(call disp_title,Cleaning,$(CLEAN_COLOR_T)); \
		fi
	@$(call exec_color,"\033[38;5;$(CLEAN_COLOR_T)m➤ \033[38;5;$(CLEAN_COLOR)m",\
		rm -f, $(NAME)) # <- Cleaning binary
	@$(eval ALREADY_RM=x)

#	All removing then compiling
re: fclean all

#	Checking norme
norme:
	@$(call disp_title,Norme,$(NORM_COLOR_T))
	@norminette $(NORME) | sed "s/Norme/[0;1;$(NORM_COLOR_T)m➤ [0;38;5;$(NORM_COLOR)mNorme/g;s/Warning/[38;5;$(NORM_COLOR_ERR)mWarning/g;s/Error/[38;5;$(NORM_COLOR_ERR)mError/g"

run: $(NAME)
	@echo "\033[1;35;7m[  Running #$(MAKELEVEL)  ]\033[0m"
	@echo "\033[0;1;35m➤ \033[0;38;5;147m./a.out ${ARGS}\033[0m"
	@./a.out ${ARGS}

codesize:
	@cat $(NORME) |grep -v '/\*' |wc -l

coffee:
	@clear
	@echo ""
	@echo "                   ("
	@echo "	                     )     ("
	@echo "               ___...(-------)-....___"
	@echo '           .-""       )    (          ""-.'
	@echo "      .-''''|-._             )         _.-|"
	@echo '     /  .--.|   `""---...........---""`   |'
	@echo "    /  /    |                             |"
	@echo "    |  |    |                             |"
	@echo "     \  \   |                             |"
	@echo "      '\ '\ |                             |"
	@echo "        '\ '|                             |"
	@echo "        _/ /\                             /"
	@echo "       (__/  \                           /"
	@echo '    _..---""` \                         /`""---.._'
	@echo " .-'           \                       /          '-."
	@echo ":               '-.__             __.-'              :"
	@echo ':                  ) ""---...---"" (                :'
	@echo "\'._                '"--...___...--"'              _.'"
	@echo '   \""--..__                              __..--""/'
	@echo "     '._     """----.....______.....----"""         _.'"
	@echo '         ""--..,,_____            _____,,..--"""'''
	@echo '                      """------"""'
	@sleep 0.5
	@clear
	@echo ""
	@echo "                 ("
	@echo "	                  )      ("
	@echo "               ___..(.------)--....___"
	@echo '           .-""       )   (           ""-.'
	@echo "      .-''''|-._      (       )        _.-|"
	@echo '     /  .--.|   `""---...........---""`   |'
	@echo "    /  /    |                             |"
	@echo "    |  |    |                             |"
	@echo "     \  \   |                             |"
	@echo "      '\ '\ |                             |"
	@echo "        '\ '|                             |"
	@echo "        _/ /\                             /"
	@echo "       (__/  \                           /"
	@echo '    _..---""` \                         /`""---.._'
	@echo " .-'           \                       /          '-."
	@echo ":               '-.__             __.-'              :"
	@echo ':                  ) ""---...---"" (                :'
	@echo "\'._                '"--...___...--"'              _.'"
	@echo '   \""--..__                              __..--""/'
	@echo "     '._     """----.....______.....----"""         _.'"
	@echo '         ""--..,,_____            _____,,..--"""'''
	@echo '                      """------"""'
	@sleep 0.5
	@clear
	@echo ""
	@echo "               ("
	@echo "	                  )     ("
	@echo "               ___..(.------)--....___"
	@echo '           .-""      )    (           ""-.'
	@echo "      .-''''|-._      (       )        _.-|"
	@echo '     /  .--.|   `""---...........---""`   |'
	@echo "    /  /    |                             |"
	@echo "    |  |    |                             |"
	@echo "     \  \   |                             |"
	@echo "      '\ '\ |                             |"
	@echo "        '\ '|                             |"
	@echo "        _/ /\                             /"
	@echo "       (__/  \                           /"
	@echo '    _..---""` \                         /`""---.._'
	@echo " .-'           \                       /          '-."
	@echo ":               '-.__             __.-'              :"
	@echo ':                  ) ""---...---"" (                :'
	@echo "\'._                '"--...___...--"'              _.'"
	@echo '   \""--..__                              __..--""/'
	@echo "     '._     """----.....______.....----"""         _.'"
	@echo '         ""--..,,_____            _____,,..--"""'''
	@echo '                      """------"""'
	@sleep 0.5
	@clear
	@echo ""
	@echo "             (         ) "
	@echo "	              )        ("
	@echo "               ___)...----)----....___"
	@echo '           .-""      )    (           ""-.'
	@echo "      .-''''|-._      (       )        _.-|"
	@echo '     /  .--.|   `""---...........---""`   |'
	@echo "    /  /    |                             |"
	@echo "    |  |    |                             |"
	@echo "     \  \   |                             |"
	@echo "      '\ '\ |                             |"
	@echo "        '\ '|                             |"
	@echo "        _/ /\                             /"
	@echo "       (__/  \                           /"
	@echo '    _..---""` \                         /`""---.._'
	@echo " .-'           \                       /          '-."
	@echo ":               '-.__             __.-'              :"
	@echo ':                  ) ""---...---"" (                :'
	@echo "\'._                '"--...___...--"'              _.'"
	@echo '   \""--..__                              __..--""/'
	@echo "     '._     """----.....______.....----"""         _.'"
	@echo '         ""--..,,_____            _____,,..--"""'''
	@echo '                      """------"""'

.PHONY: all clean fclean re norme codesize coffee
