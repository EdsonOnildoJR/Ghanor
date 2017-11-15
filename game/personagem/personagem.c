int const PTS_ATRIBUTO = 30;

enum { RACA_HUMANO = 1, RACA_SINTOZOIDE };
enum { CLASSE_SOLDADO = 1, CLASSE_ATIRADOR };

typedef struct identidade
{
    int id;
    char nome[17];
    int raca;
    int classe;
} IDENTIDADE;

typedef struct influencia
{
    int humano;
    int sintozoide;
} INFLUENCIA;

typedef struct atributo
{
    int hp;
    int xp;
    int forca;
    int agilidade;
    int inteligencia;
} ATRIBUTO;

typedef struct personagem
{
    IDENTIDADE identidade;
    INFLUENCIA influencia;
    ATRIBUTO atributo;
} Personagem;

void personagem_init(Personagem *personagem)
{
    personagem->identidade.id = contar_numero_de_personagens() + 1;
    personagem->identidade.nome[0] = '\0';
    personagem->identidade.raca = 0;
    personagem->identidade.classe = 0;
}

void save_personagem(FILE *file, Personagem *personagem, int id)
{
    fprintf(file, "Id: %d\n", id);
    fprintf(file, "Nome: %s\n", personagem->identidade.nome);
    fprintf(file, "Raca: %d\n", personagem->identidade.raca);
    fprintf(file, "Profissao: %d\n", personagem->identidade.classe);

    fprintf(file, "Influencia IA: %d\n", personagem->influencia.sintozoide);
    fprintf(file, "Influencia Humano: %d\n", personagem->influencia.humano);

    fprintf(file, "HP: %d\n", personagem->atributo.hp);
    fprintf(file, "XP: %d\n", personagem->atributo.xp);
    fprintf(file, "Forca: %d\n", personagem->atributo.forca);
    fprintf(file, "Agilidade: %d\n", personagem->atributo.agilidade);
    fprintf(file, "Inteligencia: %d\n", personagem->atributo.inteligencia);
}

void edit_personagem(Personagem *personagem)
{
    printf("\n\nEDITAR:\n\n");

    printf("NOVO NOME: ");
    scanf(" %[^\n]", personagem->identidade.nome);

    char sFile[80];

    sprintf(sFile, "data/files/p%d.txt", personagem->identidade.id);

    FILE *file = fopen(sFile, "w");

    if (file == NULL)
    {
        perror("Falha ao criar o arquivotnagens.txt");
        exit(0);
    }

    save_personagem(file, personagem, personagem->identidade.id);

    fclose(file);
}

void delete_personagem(Personagem **personagem, int id)
{
    int qnt = contar_numero_de_personagens();

    char string[80], string2[80];

    for (int i = id; i < qnt; i++)
    {
        sprintf(string, "data/personagens/p%d.txt", i);
        sprintf(string2, "data/files/p%d.txt", i + 1);

        remove(string);
        rename(string2, string);

        FILE *file = fopen(string, "w");

        if (file == NULL)
        {
            perror("Falha ao criar o arquivot /data/personagens/***.txt");
            exit(0);
        }

        save_personagem(file, personagem[i], personagem[i]->identidade.id - 1);

        fclose(file);
    }

    sprintf(string, "data/personagens/p%d.txt", qnt);
    remove(string);

    atualizar_numero_de_personagens(qnt - 1);
}

Personagem **load_personagens()
{
    int i, qnt = contar_numero_de_personagens();

    Personagem **personagem = (Personagem **)malloc(qnt * sizeof(Personagem *));

    for (i = 0; i < qnt; i++)
    {
        personagem[i] = (Personagem *)malloc(sizeof(Personagem));
    }

    char sFile[20];

    for (i = 0; i < qnt; i++)
    {
        sprintf(sFile, "data/files/p%d.txt", i + 1);

        FILE *file = fopen(sFile, "r");

        if (file == NULL)
        {
            perror("Falha ao abrir o arquivo");
            exit(0);
        }

        fscanf(file, " Id: %d", &personagem[i]->identidade.id);
        fscanf(file, " Nome: %[^\n]", personagem[i]->identidade.nome);
        fscanf(file, " Raca: %d", &personagem[i]->identidade.raca);
        fscanf(file, " Profissao: %d", &personagem[i]->identidade.classe);

        fscanf(file, " Influencia IA: %d\n", &personagem[i]->influencia.sintozoide);
        fscanf(file, " Influencia Humano: %d\n", &personagem[i]->influencia.humano);

        fscanf(file, " HP: %d\n", &personagem[i]->atributo.hp);
        fscanf(file, " XP: %d\n", &personagem[i]->atributo.xp);
        fscanf(file, " Forca: %d\n", &personagem[i]->atributo.forca);
        fscanf(file, " Agilidade: %d\n", &personagem[i]->atributo.agilidade);
        fscanf(file, " Inteligencia: %d\n", &personagem[i]->atributo.inteligencia);

        fclose(file);
    }

    return personagem;
}

void set_personagem_influencia(Personagem *personagem)
{
    srand(time(NULL));

    switch (personagem->identidade.raca)
    {
        case RACA_SINTOZOIDE:
            personagem->influencia.humano = rand() % 20;
            personagem->influencia.sintozoide = 30 + rand() % 20;
            break;

        case RACA_HUMANO:
            personagem->influencia.humano = 30 + rand() % 20;
            personagem->influencia.sintozoide = rand() % 20;
            break;
    }
}

void set_personagem_identidade_profissao_operario(Personagem *personagem)
{
    srand(time(NULL));

    personagem->atributo.inteligencia = rand() % PTS_ATRIBUTO;
    personagem->atributo.forca = 10 + rand() % (PTS_ATRIBUTO - 10);
    personagem->atributo.agilidade = 10 + rand() % (PTS_ATRIBUTO - 10);
}

void set_personagem_identidade_profissao_atirador(Personagem *personagem)
{
    srand(time(NULL));

    personagem->atributo.forca = rand() % PTS_ATRIBUTO;
    personagem->atributo.inteligencia = 10 + rand() % (PTS_ATRIBUTO - 10);
    personagem->atributo.agilidade = 10 + rand() % (PTS_ATRIBUTO - 10);
}

void set_personagem_identidade_profissao_soldado(Personagem *personagem)
{
    srand(time(NULL));

    personagem->atributo.inteligencia = rand() % PTS_ATRIBUTO;
    personagem->atributo.forca = 10 + rand() % (PTS_ATRIBUTO - 10);
    personagem->atributo.agilidade = 10 + rand() % (PTS_ATRIBUTO - 10);
}

void set_personagem_identidade_profissao_cientista(Personagem *personagem)
{
    srand(time(NULL));

    personagem->atributo.inteligencia = rand() % PTS_ATRIBUTO;
    personagem->atributo.agilidade = rand() % PTS_ATRIBUTO;
    personagem->atributo.forca = 15 + rand() % (PTS_ATRIBUTO - 15);
}

void set_personagem_atributo(Personagem *personagem)
{
    switch (personagem->identidade.classe)
    {
    case CLASSE_ATIRADOR:
        set_personagem_identidade_profissao_operario(personagem);
        break;

    case CLASSE_SOLDADO:
        set_personagem_identidade_profissao_soldado(personagem);
        break;
    }

    personagem->atributo.hp = 50;
    personagem->atributo.xp = 0;
}

void set_personagem(Personagem *personagem)
{
    set_personagem_atributo(personagem);
    set_personagem_influencia(personagem);

    int qnt = contar_numero_de_personagens();

    char sFile[20];

    sprintf(sFile, "data/files/p%d.txt", qnt + 1);

    FILE *file = fopen(sFile, "w+");

    if (file == NULL)
    {
        perror("Falha ao criar o arquivotnagens.txt");
        exit(0);
    }

    save_personagem(file, personagem, personagem->identidade.id);

    fclose(file);

    atualizar_numero_de_personagens(qnt + 1);
}