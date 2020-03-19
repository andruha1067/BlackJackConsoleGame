.PHONY: clean All

All:
	@echo "----------Building project:[ BlackJack - Debug ]----------"
	@cd "BlackJack" && "$(MAKE)" -f  "BlackJack.mk"
clean:
	@echo "----------Cleaning project:[ BlackJack - Debug ]----------"
	@cd "BlackJack" && "$(MAKE)" -f  "BlackJack.mk" clean
