# Football Highlight Wizard

A C++ application to identify potential highlights from a football (soccer) game event log.

## Description

Este programa lê um arquivo de texto contendo eventos com marcação de tempo de uma partida de futebol e identifica momentos-chave como gols, cartões vermelhos, etc., exibindo-os como destaques.

## Features (Planned/Implemented)

*   [x] Lê eventos da partida a partir de um arquivo de texto semelhante a CSV.
*   [x] Identifica destaques básicos (Gols, Cartões Vermelhos).
*   [ ] Argumentos de linha de comando para arquivos de entrada/saída.
*   [ ] Command-line arguments for input/output files.
*   [ ] (Futuro) Integração com marcações de tempo de vídeo.
*   [ ] (Futuro) Interface gráfica (GUI).

## Input File Format

O arquivo de entrada deve ser um arquivo de texto onde cada linha representa um evento da partida no seguinte formato:

`<minute>,<EVENT_TYPE>,<description>`

Exemplo (`events.log`):

Supported `EVENT_TYPE`s atualmente suportados: `GAME_START`, `HALF_TIME`, `FULL_TIME`, `GOAL`, `YELLOW_CARD`, `RED_CARD`, `FOUL`, `SUBSTITUTION`, `OTHER`.

## Building

Este projeto utiliza CMake.

1.  **Clone o repositório:**
    ```bash
    git clone <your-repo-url>
    cd Football-Highlight-Wizard
    ```
2.  **Configure usando CMake:**
    ```bash
    cmake -S . -B build
    ```
3.  **Compile o projeto:**
    ```bash
    cmake --build build
    ```
    (Alternativamente, em Linux/macOS: `cd build && make`)
    (Alternativamente, no Windows com Visual Studio: abra o arquivo da solução gerado no diretório `build` directory)

## Running

Após a compilação, o executável estará no diretório `build` directory (or a subdirectory like `build/Debug`).

```bash
./build/FootballHighlightWizard <path/to/your/events.log>
