#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FILENAME "produtos.txt"  // Arquivo onde os produtos serão armazenados

// Definindo a estrutura Produto
typedef struct {
    int id;
    char nome[50];
    float preco;
    int quantidade;
} Produto;

// Função para cadastrar um produto
void cadastrarProduto() {
    Produto p;
    FILE *arquivo = fopen(FILENAME, "a+");  // Abre o arquivo para adicionar e ler dados

    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return;
    }

    // Determina o próximo ID disponível
    int ultimoID = 0;
    while (fscanf(arquivo, "%d;%49[^;];%f;%d\n", &p.id, p.nome, &p.preco, &p.quantidade) != EOF) {
        if (p.id > ultimoID) {
            ultimoID = p.id;
        }
    }
    p.id = ultimoID + 1;  // Define o próximo ID

    printf("\nDigite o nome do produto: ");
    getchar();  // Limpa o buffer do teclado
    fgets(p.nome, sizeof(p.nome), stdin);
    p.nome[strcspn(p.nome, "\n")] = 0;  // Remove o caractere de nova linha

    printf("Digite o preço do produto: R$ ");
    scanf("%f", &p.preco);

    printf("Digite a quantidade em estoque: ");
    scanf("%d", &p.quantidade);

    // Escreve os dados no arquivo
    fprintf(arquivo, "%d;%s;%f;%d\n", p.id, p.nome, p.preco, p.quantidade);

    fclose(arquivo);
    printf("Produto cadastrado com sucesso! ID do produto: %d\n", p.id);
}

// Função para listar todos os produtos cadastrados
void listarProdutos() {
    Produto p;
    FILE *arquivo = fopen(FILENAME, "r");  // Abre o arquivo para leitura

    if (arquivo == NULL) {
        printf("Nenhum produto cadastrado ou erro ao abrir o arquivo.\n");
        return;
    }

    printf("\n--- Produtos Cadastrados ---\n");
    while (fscanf(arquivo, "%d;%49[^;];%f;%d\n", &p.id, p.nome, &p.preco, &p.quantidade) != EOF) {
        printf("ID: %d\n", p.id);
        printf("Nome: %s\n", p.nome);
        printf("Preço: R$ %.2f\n", p.preco);
        printf("Quantidade em estoque: %d\n", p.quantidade);
        printf("---------------------------\n");
    }

    fclose(arquivo);
}

// Função para buscar um produto pelo ID
void buscarProduto() {
    int idBusca;
    Produto p;
    int encontrado = 0;
    FILE *arquivo = fopen(FILENAME, "r");

    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo ou nenhum produto cadastrado.\n");
        return;
    }

    printf("\nDigite o ID do produto que deseja buscar: ");
    scanf("%d", &idBusca);

    while (fscanf(arquivo, "%d;%49[^;];%f;%d\n", &p.id, p.nome, &p.preco, &p.quantidade) != EOF) {
        if (p.id == idBusca) {
            printf("\nProduto encontrado!\n");
            printf("ID: %d\n", p.id);
            printf("Nome: %s\n", p.nome);
            printf("Preço: R$ %.2f\n", p.preco);
            printf("Quantidade em estoque: %d\n", p.quantidade);
            encontrado = 1;
            break;
        }
    }

    if (!encontrado) {
        printf("Produto não encontrado.\n");
    }

    fclose(arquivo);
}

// Função para excluir um produto usando o ID
void excluirProduto() {
    int idExcluir;
    Produto p;
    int excluido = 0;

    FILE *arquivo = fopen(FILENAME, "r");
    FILE *temp = fopen("temp.txt", "w");  // Arquivo temporário para armazenar os dados sem o produto excluído

    if (arquivo == NULL || temp == NULL) {
        printf("Erro ao abrir os arquivos.\n");
        return;
    }

    printf("\nDigite o ID do produto que deseja excluir: ");
    scanf("%d", &idExcluir);

    // Lê cada linha do arquivo original
    while (fscanf(arquivo, "%d;%49[^;];%f;%d\n", &p.id, p.nome, &p.preco, &p.quantidade) == 4) {
        // Verifica se o ID lido é diferente do ID a ser excluído
        if (p.id != idExcluir) {
            // Se o ID não coincide, grava no arquivo temporário
            fprintf(temp, "%d;%s;%f;%d\n", p.id, p.nome, p.preco, p.quantidade);
        } else {
            // Marca que o produto foi encontrado e será excluído
            excluido = 1;
        }
    }

    fclose(arquivo);
    fclose(temp);

    // Se o produto foi excluído, substitui o arquivo original pelo temporário
    if (excluido) {
        remove(FILENAME);
        rename("temp.txt", FILENAME);
        printf("Produto excluído com sucesso.\n");
    } else {
        // Remove o arquivo temporário caso o produto não tenha sido encontrado
        remove("temp.txt");
        printf("Produto não encontrado para exclusão.\n");
    }
}

// Função para realizar a venda de produtos
void venderProduto() {
    int idVenda, quantidadeVenda;
    Produto p;
    int encontrado = 0;
    float total = 0.0;

    FILE *arquivo = fopen(FILENAME, "r");
    FILE *temp = fopen("temp.txt", "w");  // Arquivo temporário para atualizar o estoque

    if (arquivo == NULL || temp == NULL) {
        printf("Erro ao abrir os arquivos.\n");
        return;
    }

    printf("\nDigite o ID do produto que deseja vender: ");
    scanf("%d", &idVenda);
    printf("Digite a quantidade que deseja vender: ");
    scanf("%d", &quantidadeVenda);

    // Lê cada linha do arquivo original e copia para o temporário, ajustando o estoque
    while (fscanf(arquivo, "%d;%49[^;];%f;%d\n", &p.id, p.nome, &p.preco, &p.quantidade) != EOF) {
        if (p.id == idVenda) {
            if (p.quantidade >= quantidadeVenda) {
                p.quantidade -= quantidadeVenda;
                total += p.preco * quantidadeVenda;
                encontrado = 1;
                printf("Venda realizada com sucesso! Valor: R$ %.2f\n", p.preco * quantidadeVenda);
            } else {
                printf("Quantidade em estoque insuficiente.\n");
            }
        }
        fprintf(temp, "%d;%s;%f;%d\n", p.id, p.nome, p.preco, p.quantidade);
    }

    if (!encontrado) {
        printf("Produto não encontrado para venda.\n");
    }

    fclose(arquivo);
    fclose(temp);

    // Substitui o arquivo original pelo temporário atualizado
    remove(FILENAME);
    rename("temp.txt", FILENAME);

    if (encontrado) {
        printf("Total da venda: R$ %.2f\n", total);
    }
}

int main() {
    int opcao;

    do {
        printf("\n--- Menu ---\n");
        printf("1. Cadastrar Produto\n");
        printf("2. Listar Produtos\n");
        printf("3. Buscar Produto\n");
        printf("4. Excluir Produto\n");
        printf("5. Vender Produto\n");
        printf("6. Sair\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);

        switch(opcao) {
            case 1:
                cadastrarProduto();
                break;
            case 2:
                listarProdutos();
                break;
            case 3:
                buscarProduto();
                break;
            case 4:
                excluirProduto();
                break;
            case 5:
                venderProduto();
                break;
            case 6:
                printf("Saindo do sistema. Até logo!\n");
                break;
            default:
                printf("Opção inválida. Tente novamente.\n");
        }

    } while(opcao != 6);

    return 0;
}