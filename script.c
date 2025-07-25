#include <stdio.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>

void imprimirNaTelaOJogoAtual();
void solicitarCelulaASerMarcada();
char retornarCaracterePorJogador(int jogador);
int celulaSelecionadaOcupada();
void verificarSeExisteVencedor();

char marcacoesDosJogadoresNaTabela[3][3] = { {' ', ' ', ' '}, {' ', ' ', ' '}, {' ', ' ', ' '} };
int jogadorAtual = 1;
int vencedor, colunaSelecionada, linhaSelecionada, opcaoSelecionada = 0;

int main(int argc, char* argv[]) {
    printf("JOGO DA VELHA\n");

    while (1 == 1) {
        printf("Selecione uma opcao: \n");
        printf("1 - Novo jogo\n");
        printf("2 - Historico de jogos\n");
        printf("3 - Sair\n");
        printf("Opcao selecionada: ");
        scanf("%d", &opcaoSelecionada);

        switch (opcaoSelecionada) {
        case 1:
            do {
                imprimirNaTelaOJogoAtual();
                solicitarCelulaASerMarcada();
                verificarSeExisteVencedor();
            } while (!vencedor);
            printf("O jogador %d venceu o jogo\n", vencedor);
            imprimirNaTelaOJogoAtual();

            FILE* arquivoHistoricoPartidasGravar = fopen("historicoDePartidas.txt", "a");

            time_t mytime = time(NULL);
            struct tm* tm = localtime(&mytime);
            char* dataHoraAtual = malloc(sizeof(char) * 13);
            strftime(dataHoraAtual, 13, "%d%m%Y%H%M", tm);

            char* str = malloc(sizeof(char) * 15);
            sprintf(str, "%s%d\n", dataHoraAtual, vencedor);
            fputs(str, arquivoHistoricoPartidasGravar);

            free(str);
            free(dataHoraAtual);
            opcaoSelecionada = 0;
            for (int i = 0; i < 3; i++) {
                for (int j = 0; j < 3; j++) {
                    marcacoesDosJogadoresNaTabela[i][j] = ' ';
                }
            }
            jogadorAtual = 1;
            vencedor = 0;
            break;
        case 2: {
            FILE* arquivoHistoricoPartidasConsultar = fopen("historicoDePartidas.txt", "r");
            char* linhaArquivo = NULL;
            while (!feof(arquivoHistoricoPartidasConsultar)) {
                linhaArquivo = malloc(sizeof(char) * 15);
                fgets(linhaArquivo, 15, arquivoHistoricoPartidasConsultar);

                if (strlen(linhaArquivo) == 3)
                    continue;

                char* dia = malloc(sizeof(char) * 2);
                sprintf(dia, "%c%c", linhaArquivo[0], linhaArquivo[1]);
                char* mes = malloc(sizeof(char) * 2);
                sprintf(mes, "%c%c", linhaArquivo[2], linhaArquivo[3]);
                char* ano = malloc(sizeof(char) * 4);
                sprintf(ano, "%c%c%c%c", linhaArquivo[4], linhaArquivo[5], linhaArquivo[6], linhaArquivo[7]);
                char* hora = malloc(sizeof(char) * 2);
                sprintf(hora, "%c%c", linhaArquivo[8], linhaArquivo[9]);
                char* minuto = malloc(sizeof(char) * 2);
                sprintf(minuto, "%c%c", linhaArquivo[10], linhaArquivo[11]);

                char* dataHoraJogo = malloc(sizeof(char) * 17);
                sprintf(dataHoraJogo, "%s/%s/%s %s:%s", dia, mes, ano, hora, minuto);

                printf("Data/hora: %s, jogador que venceu: %c\n", dataHoraJogo, linhaArquivo[12]);
                free(linhaArquivo);
                free(dia);
                free(mes);
                free(ano);
                free(hora);
                free(minuto);
                free(dataHoraJogo);
            }
            opcaoSelecionada = 0;
            break;
        }
        case 3:
            printf("Ate mais\n");
            return 0;
        default:
            printf("Opcao selecionada nao existe\n");
        }
    }

    return 0;
}

void imprimirNaTelaOJogoAtual() {
    printf("%c|%c|%c\n", marcacoesDosJogadoresNaTabela[0][0], marcacoesDosJogadoresNaTabela[0][1], marcacoesDosJogadoresNaTabela[0][2]);
    printf("-+-+-\n");
    printf("%c|%c|%c\n", marcacoesDosJogadoresNaTabela[1][0], marcacoesDosJogadoresNaTabela[1][1], marcacoesDosJogadoresNaTabela[1][2]);
    printf("-+-+-\n");
    printf("%c|%c|%c\n", marcacoesDosJogadoresNaTabela[2][0], marcacoesDosJogadoresNaTabela[2][1], marcacoesDosJogadoresNaTabela[2][2]);
}

void solicitarCelulaASerMarcada() {
    int celulaOcupada = 0;

    do {
        printf("Vez do jogador %d\n", jogadorAtual);
        printf("Digite a linha: ");
        scanf("%d", &linhaSelecionada);
        printf("Digite a coluna: ");
        scanf("%d", &colunaSelecionada);
        linhaSelecionada--;
        colunaSelecionada--;

        celulaOcupada = celulaSelecionadaOcupada();

        if (celulaOcupada == 1) {
            printf("A celula selecionada ja se encontra marcada\n");
            colunaSelecionada = 0;
            linhaSelecionada = 0;

        }
    } while (celulaOcupada == 1);

    marcacoesDosJogadoresNaTabela[linhaSelecionada][colunaSelecionada] = retornarCaracterePorJogador(jogadorAtual);

    jogadorAtual = jogadorAtual == 1 ? 2 : 1;
}

char retornarCaracterePorJogador(int jogador) {
    return jogador == 1 ? 'X' : 'O';
}

int celulaSelecionadaOcupada() {
    return marcacoesDosJogadoresNaTabela[linhaSelecionada][colunaSelecionada] != ' ';
}

void verificarSeExisteVencedor() {
    for (int jogador = 1; jogador < 3; jogador++) {
        char caractereJogador = retornarCaracterePorJogador(jogador);
        for (int posicao = 0; posicao < 3; posicao++) {
            if (
                (
                    marcacoesDosJogadoresNaTabela[posicao][0] == caractereJogador &&
                    marcacoesDosJogadoresNaTabela[posicao][1] == caractereJogador &&
                    marcacoesDosJogadoresNaTabela[posicao][2] == caractereJogador
                    ) ||
                (
                    marcacoesDosJogadoresNaTabela[0][posicao] == caractereJogador &&
                    marcacoesDosJogadoresNaTabela[1][posicao] == caractereJogador &&
                    marcacoesDosJogadoresNaTabela[2][posicao] == caractereJogador
                    )
                ) {
                vencedor = jogador;
            }
        }

        if (
            (
                marcacoesDosJogadoresNaTabela[0][0] == caractereJogador &&
                marcacoesDosJogadoresNaTabela[1][1] == caractereJogador &&
                marcacoesDosJogadoresNaTabela[2][2] == caractereJogador
                ) ||
            (
                marcacoesDosJogadoresNaTabela[0][2] == caractereJogador &&
                marcacoesDosJogadoresNaTabela[1][1] == caractereJogador &&
                marcacoesDosJogadoresNaTabela[2][0] == caractereJogador
                )
            ) {
            vencedor = jogador;
        }
    }
}
