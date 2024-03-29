/**
 * @file      mips_isa.cpp
 * @author    Sandro Rigo
 *            Marcus Bartholomeu
 *            Alexandro Baldassin (acasm information)
 *
 *            The ArchC Team
 *            http://www.archc.org/
 *
 *            Computer Systems Laboratory (LSC)
 *            IC-UNICAMP
 *            http://www.lsc.ic.unicamp.br/
 *
 * @version   1.0
 * @date      Mon, 19 Jun 2006 15:50:52 -0300
 *
 * @brief     The ArchC i8051 functional model.
 *
 * @attention Copyright (C) 2002-2006 --- The ArchC Team
 *
 */

#include  <cstdlib>
#include  <iostream>
#include  "mips_isa.H"
#include  "mips_isa_init.cpp"
#include  "mips_bhv_macros.H"
#include <vector>

extern "C" {
#include "d4.h"
}

//If you want debug information for this model, uncomment next line
//#define DEBUG_MODEL
#include "ac_debug_model.H"


//!User defined macros to reference registers.
#define Ra 31
#define Sp 29

// 'using namespace' statement to allow access to all
// mips-specific datatypes
using namespace mips_parms;

static int processors_started = 0;
#define DEFAULT_STACK_SIZE (256*1024)

// Cache descriptors
d4cache* DataMem;
d4cache* InstrMem;
d4cache* DL1;
d4cache* DL2;
d4cache* IL1;
d4cache* IL2;

#define I_SIZE 4

#ifndef _L1_CACHE_LG2_SIZE
    #define _L1_CACHE_LG2_SIZE 16
#endif

#ifndef _L1_CACHE_LG2_B_SIZE
    #define _L1_CACHE_LG2_B_SIZE 3
#endif

#ifndef _L2_CACHE_LG2_SIZE
    #define _L2_CACHE_LG2_SIZE 18
#endif

#ifndef _L2_CACHE_LG2_B_SIZE
    #define _L2_CACHE_LG2_B_SIZE 7
#endif

#ifndef _CACHE_ASSOC
    #define _CACHE_ASSOC 2
#endif


/* ~~~ Implementação do Branch Prediction e Pipeline ~~~
 * Primeiramente, precisamos saber qual a instrução está sendo executada e seus
 * parâmetros - por isso, temos esse struct que possui qual o tipo de instrução
 * (inst) que pode ser uma instrução do formato R, I ou J
 *      R: Usada para operações aritméticas
 *         Opcode - rs - rt - rd - shift amt - functional
 *         rs ~> src | rt ~> src | rd ~> dst
 *      I: Usada para operações de load e store e jump condicional
 *         opcode - rs - rt - address
 *         rs ~> base| rt ~> dst
 *      J: Usada para jump não condicional
 *         Opcode - address
 * Algumas obs:
 *  - Inst com imediato usa instrução do tipo I
 * mais info: homepage.cs.uiowa.edu/~ghosh/1-24-06.pdf
 *
 *
 * Jumps são resolvidos no segundo estãgio
 * ~~~~ Explicações dos pipelines ~~~~
 * ~~ Pipeline de 5 Estágios ~~
 * ~ Escalar
 * | IF | ID | EX | MEM | WB |
 *  Data Hazards nos seguintes casos (Sem Fowarding):
 *    - EX|MEM.rd = ID/EX.rs ou ID/EX.rt - 2 stalls
 *    - MEM|WB.rd = ID/EX.rs ou ID/EX.rt - 1 stalls
 *  Control Hazards:
 *    - Sem Branch Prediction:
 *      - O pipeline para a execução até calcular o branch - 3 stalls
 *    - Always not Taken
 *      - Se tomar o branch - 4 stalls
 *      - Se não tomar      - 0 stalls
 *    - Two-bit (não toma <-> não toma <-> toma <-> toma )
 *      - Se nao tomar e for para tomar e vice e versa - 4 stalls
 *
 * ~ Superescalar de dois níveis in-order-commit
 * | IF | ID | EX | MEM | WB |
 * | IF | ID | EX | MEM | WB |
 *  Data Hazards nos seguintes casos (Sem Fowarding):
 *    -  1EX|1MEM.rd = 1ID/1EX.rs ou 1ID/1EX.rt - 2 stalls-|
 *    -  1MEM|1WB.rd = 1ID/1EX.rs ou 1ID/1EX.rt - 1 stalls |\ Casos escalar
 *    -  2EX|2MEM.rd = 2ID/2EX.rs ou 2ID/2EX.rt - 2 stalls |/
 *    -  2MEM|2WB.rd = 2ID/2EX.rs ou 2ID/2EX.rt - 1 stalls-|
 *    Como eles devem estar alinhados (ioc) dobra os números de stalls
 *    -  1EX|1MEM.rd = 2ID|2EX.rs ou 2ID/2EX.rt - 4 stalls
 *    -  1MEM|1WB.rd = 2ID/2EX.rs ou 2ID/2EX.rt - 2 stalls
 *    -  2EX|2MEM.rd = 1ID/1EX.rs ou 1ID/1EX.rt - 4 stalls
 *    -  2MEM|2WB.rd = 1ID/1EX.rs ou 1ID/1EX.rt - 2 stalls
 *
 * ~~ Pipeline de 7 estágios ~~
 * | IT | IF | ID | EX | MT | MM | WB |
 *  Data Hazards nos seguintes casos (Sem Fowarding):
 *    - EX|MT.rd = ID|EX.rs ou ID|EX.rt - 3 stalls
 *    - MT|MM.rd = ID|EX.rs ou ID|EX.rt - 2 stalls
 *    - MM|WB.rd = ID|EX.rs ou ID|EX.rt - 1 stalls
 *  Control Hazards:
 *    - Sem Branch Prediction:
 *      - O pipeline para a execução até calcular o branch - 6 stalls
 *
 * ~~ Pipeline de 13 estágios ~~
 * Baseado no Cortex A8 pipeline de 13 estágios
 * Vamos não fazer ....?....
 * | F1 | F2 | D0 | D1 | D2 | D3 | D4 | E0 | E1 | E2 | E3 | E4 | E5 |
 *  Data Hazards nos seguintes casos (Sem Fowarding):
 *    -
 *  Control Hazards:
 *    -
 */

#ifndef _PIPELINE_SIZE
    #define _PIPELINE_SIZE 5      // 5 , 7 ou 13
#endif

#ifndef _PIPELINE_TYPE
    #define _PIPELINE_TYPE 1      // 1 = Escalar ; 2 = Superescalar
#endif

#ifndef _BRANCH_PRED
    #define _BRANCH_PRED   0      // 0 = Sem ; 1 = always not taken ; 2 = 2 bit
#endif

/* Define as posições no vetor */
/* 5 estágios */
#define IF5  0
#define ID5  1
#define EX5  2
#define MEM5 3
#define WB5  4
/* 7 estágios */
#define IT7  0
#define IF7  1
#define ID7  2
#define EX7  3
#define MT7  4
#define MM7  5
#define WB7  6

enum Inst_format { Rtype, Itype, Jtype };

typedef struct {
    Inst_format type;
    int rs, rt, rd;
} Instruction;

void initialize_pipeline();
Instruction createInst(Inst_format type, int rs, int rt, int rd);
void data_hazards_pipeline();
void insert_inst_pipeline(Instruction newinst);
void branch_taken_pipeline();
void branch_not_taken_pipeline();
void print_status();

/* Valores que queremos de saída */
unsigned long int instr, instr_R, instr_I, instr_J;    // Número de instruções
unsigned long int data_hazards, control_hazard;        // Numero de hazards
unsigned long int stalls, d_stalls, c_stalls, j_stalls;  // Número de stalls
unsigned long int branches, c_branches, i_branches;

/* Posição 0 - ultima instrução inserida no pipeline
 * Posicao N - Ultima do pipeline
 */
std::vector<Instruction> pipeline(_PIPELINE_SIZE), pipeline2(_PIPELINE_SIZE);

void initialize_pipeline() {
    instr = 0;
    instr_R = 0;
    instr_I = 0;
    instr_J = 0;
    data_hazards = 0;
    control_hazard = 0;
    stalls = 0;
    d_stalls = 0;
    c_stalls = 0;
    j_stalls = 0;
    branches = 0;
    c_branches = 0;
    i_branches = 0;
}

Instruction createInst(Inst_format type, int rs, int rt, int rd) {
    Instruction newinst;
    newinst.type = type;
    newinst.rs = rs;
    newinst.rt = rt;
    newinst.rd = rd;

    return newinst;
}

/* Insere uma nova instrução no pipeline */
bool oneOrTwo = false;
void insert_inst_pipeline(Instruction newinst) {
    /* Contador de # intruções */
    instr += 1;
    if (newinst.type == Rtype) {
        instr_R += 1;
    } else if (newinst.type == Itype) {
        instr_I += 1;
    } else if (newinst.type == Jtype) {
        instr_J += 1;

        stalls += 2;
        j_stalls += 2;
    }

    /* Adiciona essa instrução no pipeline */
    if (_PIPELINE_TYPE == 1) {
        for (int i = pipeline.size(); i > 1; i--) {
            pipeline[i-1] = pipeline[i-2];
        }
        pipeline[0] = newinst;

        if (instr >= _PIPELINE_SIZE-1) {
            data_hazards_pipeline();
        }
    } else if (_PIPELINE_TYPE == 2) {
        /* No escalar, só verifica se tem hazard quando os dois estao comp. */
        if (oneOrTwo == false) {
            for (int i = pipeline.size(); i > 1; i--) {
                pipeline[i-1] = pipeline[i-2];
            }
            pipeline[0] = newinst;

            oneOrTwo = true;
        } else {
            for (int i = pipeline2.size(); i > 1; i--) {
                pipeline2[i-1] = pipeline2[i-2];
            }
            pipeline2[0] = newinst;

            if (instr >= _PIPELINE_SIZE-1) {
                data_hazards_pipeline();
            }

            oneOrTwo = false;
        }
    }
}


/* Implementa toda a lógica do pipeline */
void data_hazards_pipeline() {
    if (_PIPELINE_SIZE == 5) {
        if (_PIPELINE_TYPE == 1) {
            /* Pipeline escalar de 5 estágios */
            if (pipeline[MEM5].rd == pipeline[ID5].rs) {
                /* Um stalls */
                stalls += 1;
                d_stalls += 1;
                data_hazards += 1;
            } else if (pipeline[MEM5].rd == pipeline[ID5].rt) {
                /* Um stall */
                stalls += 1;
                d_stalls += 1;
                data_hazards += 1;
            } else if (pipeline[EX5].rd == pipeline[ID5].rs) {
                /* Dois stalls */
                stalls += 2;
                d_stalls += 2;
                data_hazards += 1;
            } else if (pipeline[EX5].rd == pipeline[ID5].rt)  {
                /* Dois stalls */
                stalls += 2;
                d_stalls += 2;
                data_hazards += 1;
            }

        } else if (_PIPELINE_TYPE == 2) {
            /* Pipeline Superescalar de 5 estágios */

            /* Pipeline 1 */
            if (pipeline[EX5].rd == pipeline[ID5].rs) {
                /* Dois stalls */
                stalls += 2;
                d_stalls += 2;
                data_hazards += 1;
            } else if (pipeline[EX5].rd == pipeline[ID5].rt)  {
                /* Dois stalls */
                stalls += 2;
                d_stalls += 2;
                data_hazards += 1;
            } else if (pipeline[MEM5].rd == pipeline[ID5].rs) {
                /* Um stalls */
                stalls += 1;
                d_stalls += 1;
                data_hazards += 1;
            } else if (pipeline[MEM5].rd == pipeline[ID5].rt) {
                /* Um stall */
                stalls += 1;
                d_stalls += 1;
                data_hazards += 1;
            }

            /* Pipeline 2 */
            if (pipeline2[EX5].rd == pipeline2[ID5].rs) {
                /* Dois stalls */
                stalls += 2;
                d_stalls += 2;
                data_hazards += 1;
            } else if (pipeline2[EX5].rd == pipeline2[ID5].rt)  {
                /* Dois stalls */
                stalls += 2;
                d_stalls += 2;
                data_hazards += 1;
            } else if (pipeline2[MEM5].rd == pipeline2[ID5].rs) {
                /* Um stalls */
                stalls += 1;
                d_stalls += 1;
                data_hazards += 1;
            } else if (pipeline2[MEM5].rd == pipeline2[ID5].rt) {
                /* Um stall */
                stalls += 1;
                d_stalls += 1;
                data_hazards += 1;
            }

            /* Merge deles */
            if (pipeline[EX5].rd == pipeline2[ID5].rs) {
                /* 2 stalls */
                stalls += 2;
                d_stalls += 2;
                data_hazards += 1;
            } else if (pipeline[EX5].rd == pipeline2[ID5].rt) {
                /* 2 stalls */
                stalls += 2;
                d_stalls += 2;
                data_hazards += 1;
            } else if (pipeline[MEM5].rd == pipeline2[ID5].rs) {
                /* 1 stalls */
                stalls += 1;
                d_stalls += 1;
                data_hazards += 1;
            } else if (pipeline[MEM5].rd == pipeline2[ID5].rt) {
                /* 1 stalls */
                stalls += 1;
                d_stalls += 1;
                data_hazards += 1;
            } else if (pipeline2[EX5].rd == pipeline[ID5].rs) {
                /* 2 stalls */
                stalls += 2;
                d_stalls += 2;
                data_hazards += 1;
            } else if (pipeline2[EX5].rd == pipeline[ID5].rt) {
                /* 2 stalls */
                stalls += 2;
                d_stalls += 2;
                data_hazards += 1;
            } else if (pipeline2[MEM5].rd == pipeline[ID5].rs) {
                /* 1 stalls */
                stalls += 1;
                d_stalls += 1;
                data_hazards += 1;
            } else if (pipeline2[MEM5].rd == pipeline[ID5].rt) {
                /* 1 stalls */
                stalls += 1;
                d_stalls += 1;
                data_hazards += 1;
            }
        }
    } else if (_PIPELINE_SIZE == 7) {
        /* Pipeline escalar de 7 estágios */
        if (pipeline[EX7].rd == pipeline[ID7].rs) {
            /* 3 stalls */
            stalls += 3;
            d_stalls += 3;
            data_hazards += 1;
        } else if (pipeline[EX7].rd == pipeline[ID7].rt) {
            /* 3 stalls */
            stalls += 3;
            d_stalls += 3;
            data_hazards += 1;
        } else if (pipeline[MT7].rd == pipeline[ID7].rs) {
            /* 2 stalls */
            stalls += 2;
            d_stalls += 2;
            data_hazards += 1;
        } else if (pipeline[MT7].rd == pipeline[ID7].rt) {
            /* 2 stalls */
            stalls += 2;
            d_stalls += 2;
            data_hazards += 1;
        } else if (pipeline[MM7].rd == pipeline[ID7].rs) {
            /* 1 stalls */
            stalls += 1;
            d_stalls += 1;
            data_hazards += 1;
        } else if (pipeline[MM7].rd == pipeline[ID7].rt) {
            /* 1 stalls */
            stalls += 1;
            d_stalls += 1;
            data_hazards += 1;
        }
    }
    return;
}

/* Se for 0 ou 1 ele não deveria saltar
 * Se for 2 ou 3 ele deveria saltar
 */
int twobitprediction = 0;

/* Funcao chamada quando um branch é executado */
void branch_taken_pipeline() {
    branches += 1;
    if (_PIPELINE_SIZE == 5) {
        if (_BRANCH_PRED == 0) {
            /* _PIPELINE_SIZE-1 Stalls */
            stalls += _PIPELINE_SIZE-2;
            c_stalls += _PIPELINE_SIZE-2;
            control_hazard += 1;
        } else if (_BRANCH_PRED == 1) {
            /* _PIPELINE_SIZE-1 Stalls */
            stalls += _PIPELINE_SIZE-2;
            c_stalls += _PIPELINE_SIZE-2;
            control_hazard += 1;
            i_branches += 1;
        } else if (_BRANCH_PRED == 2) {
            if (twobitprediction == 0 || twobitprediction == 1) {
                /* _PIPELINE_SIZE-1 Stalls */
                stalls += _PIPELINE_SIZE-2;
                c_stalls += _PIPELINE_SIZE-2;
                control_hazard += 1;
                i_branches += 1;
            } else {
                // Acertou
                c_branches += 1;
            }
            /* Atualiza o valor do 2bit */
            if (twobitprediction >= 0 && twobitprediction < 3) {
                twobitprediction += 1;
            }
        }
    } else if (_PIPELINE_SIZE == 7) {
        /* Não tem branch prediction */
        /* _PIPELINE_SIZE-1 Stalls */
        stalls += _PIPELINE_SIZE-2;
        c_stalls += _PIPELINE_SIZE-2;
        control_hazard += 1;
    }
}

/* Funcão chamada quando um branch não é executado */
void branch_not_taken_pipeline() {
    branches += 1;
    if (_PIPELINE_SIZE == 5) {
        if (_BRANCH_PRED == 0) {
            /* _PIPELINE_SIZE-1 Stalls */
            stalls += _PIPELINE_SIZE-2;
            c_stalls += _PIPELINE_SIZE-2;
            control_hazard += 1;
        } else if (_BRANCH_PRED == 1) {
            c_branches += 1;
        } else  if (_BRANCH_PRED == 2) {
            if (twobitprediction == 2 || twobitprediction == 3) {
                /* _PIPELINE_SIZE-1 Stalls */
                stalls += _PIPELINE_SIZE-1;
                c_stalls += _PIPELINE_SIZE-1;
                control_hazard += 1;
                i_branches += 1;

                twobitprediction -= 1;
            } else {
                // Acertou
                c_branches += 1;
            }

            /* Atualiza o valor do 2bit */
            if (twobitprediction > 0 && twobitprediction <= 3) {
                twobitprediction -= 1;
            }
        }
    }
}


void print_status() {
    int cycles;
    double CPI;

    if (_PIPELINE_TYPE == 1) {
        cycles = stalls + instr + _PIPELINE_SIZE - 1;
    } else {
        cycles = stalls + (instr/2) + _PIPELINE_SIZE - 1;
    }

    CPI = static_cast<double> cycles/ static_cast<double> instr;

    std::cout   << "Pipeline: " << _PIPELINE_SIZE
                << "\tTipo: " << _PIPELINE_TYPE << "\tBP: "
                << _BRANCH_PRED << endl;
    std::cout << "Cycles: " << cycles << endl;
    std::cout << "Instructions: " << instr << endl;
    std::cout << "Instructions R: " << instr_R << endl;
    std::cout << "Instructions I: " << instr_I << endl;
    std::cout << "Instructions J: " << instr_J << endl;
    std::cout << "CPI: " <<  CPI << endl;
    std::cout << "Data Hazards: " << data_hazards << endl;
    std::cout << "Control Hazards: " << control_hazard << endl;
    std::cout << "Stalls: " << stalls << endl;
    std::cout << "Data Stalls: " << d_stalls << endl;
    std::cout << "Control Stalls: " << c_stalls << endl;
    std::cout << "Jump Stalls: " << j_stalls << endl;
    std::cout << "Branches: " << branches << endl;
    std::cout << "Correct Branches: " << c_branches << endl;
    std::cout << "Incorrect Branches: " << i_branches << endl;
}

// ----------------------- CACHE FUNCTIONS --------------------------------
// Read operation on the cache
void doread(unsigned int addr, size_t size, d4cache* Cache) {
    d4memref R;
    R.address = (d4addr)addr;
    R.size = size;
    R.accesstype = D4XREAD;
    d4ref(Cache, R);
}

// Write operation on the cache
void dowrite(unsigned int addr, size_t size, d4cache* Cache) {
    d4memref R;
    R.address = (d4addr)addr;
    R.size = size;
    R.accesstype = D4XWRITE;
    d4ref(Cache, R);
}

void doinstrn(unsigned int addr, size_t size, d4cache* Cache) {
    d4memref R;
    R.address = (d4addr)addr;
    R.size = size;
    R.accesstype = D4XINSTRN;
    d4ref(Cache, R);
}

void print_cache_line(double m, double f, char * name) {

    std::cout << name << m << " of " << f << " --> ";

    if (f != 0)
        std::cout << (m * 100)/f << " %" << endl;
    else
        std::cout << "0 %" << endl;
}

//!Generic instruction behavior method.
void ac_behavior( instruction )
{
    doinstrn(ac_pc, I_SIZE, IL1);
    dbg_printf("----- PC=%#x ----- %lld\n", (int) ac_pc, ac_instr_counter);
    //  dbg_printf("----- PC=%#x NPC=%#x ----- %lld\n", (int) ac_pc, (int)npc, ac_instr_counter);
#ifndef NO_NEED_PC_UPDATE
    ac_pc = npc;
    npc = ac_pc + 4;
#endif
};

//! Instruction Format behavior methods.
void ac_behavior( Type_R ){}
void ac_behavior( Type_I ){}
void ac_behavior( Type_J ){}

//!Behavior called before starting simulation
void ac_behavior(begin)
{
    /* Initialize the pipeline */
    initialize_pipeline();

    dbg_printf("@@@ begin behavior @@@\n");
    RB[0] = 0;
    npc = ac_pc + 4;

    // Is is not required by the architecture, but makes debug really easier
    for (int regNum = 0; regNum < 32; regNum ++)
        RB[regNum] = 0;
    hi = 0;
    lo = 0;

    RB[29] =  AC_RAM_END - 1024 - processors_started++ * DEFAULT_STACK_SIZE;

    // Setting caches
    DataMem = d4new(NULL);

    // Data L2 Cache
    DL2 = d4new(DataMem);
    DL2->name = "DL2";
    DL2->lg2blocksize = _L2_CACHE_LG2_B_SIZE;
    DL2->lg2subblocksize = 0;
    DL2->lg2size = _L2_CACHE_LG2_SIZE;
    DL2->assoc = _CACHE_ASSOC;
    DL2->replacementf = d4rep_lru;
    DL2->prefetchf = d4prefetch_none;
    DL2->wallocf = d4walloc_always;
    DL2->wbackf = d4wback_always;
    DL2->name_replacement = DL2->name_prefetch = DL2->name_walloc = DL2->name_wback = "DL2";

    // Data L1 Cache
    DL1 = d4new(DL2);
    DL1->name = "DL1";
    DL1->lg2blocksize = _L1_CACHE_LG2_B_SIZE;
    DL1->lg2subblocksize = 0;
    DL1->lg2size = _L1_CACHE_LG2_SIZE;
    DL1->assoc = _CACHE_ASSOC;
    DL1->replacementf = d4rep_lru;
    DL1->prefetchf = d4prefetch_none;
    DL1->wallocf = d4walloc_always;
    DL1->wbackf = d4wback_always;
    DL1->name_replacement = DL1->name_prefetch = DL1->name_walloc = DL1->name_wback = "DL1";


    InstrMem = d4new(NULL);
    // Instruction L2 Cache
    IL2 = d4new(InstrMem);
    IL2->name = "IL2";
    IL2->lg2blocksize = _L2_CACHE_LG2_B_SIZE;
    IL2->lg2subblocksize = 0;
    IL2->lg2size = _L1_CACHE_LG2_SIZE;
    IL2->assoc = _CACHE_ASSOC;
    IL2->replacementf = d4rep_lru;
    IL2->prefetchf = d4prefetch_none;
    IL2->wallocf = d4walloc_always;
    IL2->wbackf = d4wback_always;
    IL2->name_replacement = IL2->name_prefetch = IL2->name_walloc = IL2->name_wback = "IL2";

    // Instruction L1 Cache
    IL1 = d4new(IL2);
    IL1->name = "IL1";
    IL1->lg2blocksize = _L1_CACHE_LG2_B_SIZE;
    IL1->lg2subblocksize = 0;
    IL1->lg2size = _L1_CACHE_LG2_SIZE;
    IL1->assoc = _CACHE_ASSOC;
    IL1->replacementf = d4rep_lru;
    IL1->prefetchf = d4prefetch_none;
    IL1->wallocf = d4walloc_always;
    IL1->wbackf = d4wback_always;
    IL1->name_replacement = IL1->name_prefetch = IL1->name_walloc = IL1->name_wback = "IL1";

    int r;
    if (0 != (r = d4setup())) {
        dbg_printf("Cache Failed\n");
        exit(1);
    }


}

//!Behavior called after finishing simulation
void ac_behavior(end)
{

    double m,ms,f,fs;

    ms = fs = 0;
    std::cout << "Data L1 Cache: " << endl;

    m = DL1->miss[D4XREAD]; ms += m; f = DL1->fetch[D4XREAD]; fs += f;
    print_cache_line(m, f, "READ:     ");

    m = DL1->miss[D4XWRITE]; ms += m; f = DL1->fetch[D4XWRITE]; fs += f;
    print_cache_line(m, f, "WRITE:    ");

    m = DL1->miss[D4XREAD+D4PREFETCH]; ms += m; f = DL1->fetch[D4XREAD+D4PREFETCH]; fs += f;
    print_cache_line(m, f, "READ+PF:  ");

    m = DL1->miss[D4XWRITE+D4PREFETCH]; ms += m; f = DL1->fetch[D4XWRITE+D4PREFETCH]; fs += f;
    print_cache_line(m, f, "WRITE+PF: ");

    print_cache_line(ms, fs, "TOTAL:    ");
    std::cout << endl;


    ms = fs = 0;
    std::cout << "Data L2 Cache: "  << endl;

    m = DL2->miss[D4XREAD]; ms += m; f = DL2->fetch[D4XREAD]; fs += f;
    print_cache_line(m, f, "READ:     ");

    m = DL2->miss[D4XWRITE]; ms += m; f = DL2->fetch[D4XWRITE]; fs += f;
    print_cache_line(m, f, "WRITE:    ");

    m = DL2->miss[D4XREAD+D4PREFETCH]; ms += m; f = DL2->fetch[D4XREAD+D4PREFETCH]; fs += f;
    print_cache_line(m, f, "READ+PF:  ");

    m = DL2->miss[D4XWRITE+D4PREFETCH]; ms += m; f = DL2->fetch[D4XWRITE+D4PREFETCH]; fs += f;
    print_cache_line(m, f, "WRITE+PF: ");

    print_cache_line(ms, fs, "TOTAL:    ");
    std::cout << endl << endl;



    ms = fs = 0;
    std::cout << "Instruction L1 Cache: " << endl;

    m = IL1->miss[D4XINSTRN]; ms += m; f = IL1->fetch[D4XINSTRN]; fs += f;
    print_cache_line(m, f, "INSTR:    ");

    m = IL1->miss[D4XINSTRN+D4PREFETCH]; ms += m; f = IL1->fetch[D4XINSTRN+D4PREFETCH]; fs += f;
    print_cache_line(m, f, "INSTR+PF: ");

    print_cache_line(ms, fs, "TOTAL:    ");
    std::cout << endl;


    ms = fs = 0;
    std::cout << "Instruction L2 Cache: "  << endl;

    m = IL2->miss[D4XINSTRN]; ms += m; f = IL2->fetch[D4XINSTRN]; fs += f;
    print_cache_line(m, f, "INSTR:    ");

    m = IL2->miss[D4XINSTRN+D4PREFETCH]; ms += m; f = IL2->fetch[D4XINSTRN+D4PREFETCH]; fs += f;
    print_cache_line(m, f, "INSTR+PF: ");

    print_cache_line(ms, fs, "TOTAL:    ");
    std::cout << endl;

    print_status();

    dbg_printf("@@@ end behavior @@@\n");
}




//!Instruction lb behavior method.
void ac_behavior( lb )
{
    doread(RB[rs]+imm, sizeof(char), DL1);
    char byte;
    dbg_printf("lb r%d, %d(r%d)\n", rt, imm & 0xFFFF, rs);
    byte = DATA_PORT->read_byte(RB[rs]+ imm);
    RB[rt] = (ac_Sword)byte ;
    dbg_printf("Result = %#x\n", RB[rt]);

    insert_inst_pipeline(createInst(Itype, rs, -2, rt));
};

//!Instruction lbu behavior method.
void ac_behavior( lbu )
{
    doread(RB[rs]+imm, sizeof(unsigned char), DL1);
    unsigned char byte;
    dbg_printf("lbu r%d, %d(r%d)\n", rt, imm & 0xFFFF, rs);
    byte = DATA_PORT->read_byte(RB[rs]+ imm);
    RB[rt] = byte ;
    dbg_printf("Result = %#x\n", RB[rt]);

    insert_inst_pipeline(createInst(Itype, rs, -2, rt));
};

//!Instruction lh behavior method.
void ac_behavior( lh )
{
    doread(RB[rs]+imm, sizeof(short int), DL1);
    short int half;
    dbg_printf("lh r%d, %d(r%d)\n", rt, imm & 0xFFFF, rs);
    half = DATA_PORT->read_half(RB[rs]+ imm);
    RB[rt] = (ac_Sword)half ;
    dbg_printf("Result = %#x\n", RB[rt]);

    insert_inst_pipeline(createInst(Itype, rs, -2, rt));
};

//!Instruction lhu behavior method.
void ac_behavior( lhu )
{
    doread(RB[rs]+imm, sizeof(unsigned short int), DL1);
    unsigned short int  half;
    half = DATA_PORT->read_half(RB[rs]+ imm);
    RB[rt] = half ;
    dbg_printf("Result = %#x\n", RB[rt]);

    insert_inst_pipeline(createInst(Itype, rs, -2, rt));
};

//!Instruction lw behavior method.
void ac_behavior( lw )
{
    doread(RB[rs]+imm, sizeof(int), DL1);
    dbg_printf("lw r%d, %d(r%d)\n", rt, imm & 0xFFFF, rs);
    RB[rt] = DATA_PORT->read(RB[rs]+ imm);
    dbg_printf("Result = %#x\n", RB[rt]);

    insert_inst_pipeline(createInst(Itype, rs, -2, rt));
};

//!Instruction lwl behavior method.
void ac_behavior( lwl )
{
    doread((RB[rs]+imm) & 0xFFFFFFFC, sizeof(int), DL1);
    dbg_printf("lwl r%d, %d(r%d)\n", rt, imm & 0xFFFF, rs);
    unsigned int addr, offset;
    ac_Uword data;

    addr = RB[rs] + imm;
    offset = (addr & 0x3) * 8;
    data = DATA_PORT->read(addr & 0xFFFFFFFC);
    data <<= offset;
    data |= RB[rt] & ((1<<offset)-1);
    RB[rt] = data;
    dbg_printf("Result = %#x\n", RB[rt]);

    insert_inst_pipeline(createInst(Itype, rs, -2, rt));
};

//!Instruction lwr behavior method.
void ac_behavior( lwr )
{
    doread((RB[rs]+imm) & 0xFFFFFFFC, sizeof(int), DL1);
    dbg_printf("lwr r%d, %d(r%d)\n", rt, imm & 0xFFFF, rs);
    unsigned int addr, offset;
    ac_Uword data;

    addr = RB[rs] + imm;
    offset = (3 - (addr & 0x3)) * 8;
    data = DATA_PORT->read(addr & 0xFFFFFFFC);
    data >>= offset;
    data |= RB[rt] & (0xFFFFFFFF << (32-offset));
    RB[rt] = data;
    dbg_printf("Result = %#x\n", RB[rt]);

    insert_inst_pipeline(createInst(Itype, rs, -2, rt));
};

//!Instruction sb behavior method.
void ac_behavior( sb )
{
    dowrite(RB[rs]+imm, sizeof(unsigned char), DL1);
    unsigned char byte;
    dbg_printf("sb r%d, %d(r%d)\n", rt, imm & 0xFFFF, rs);
    byte = RB[rt] & 0xFF;
    DATA_PORT->write_byte(RB[rs] + imm, byte);
    dbg_printf("Result = %#x\n", (int) byte);

    insert_inst_pipeline(createInst(Itype, rs, rt, -3));
};

//!Instruction sh behavior method.
void ac_behavior( sh )
{
    dowrite(RB[rs]+imm, sizeof(unsigned short int), DL1);
    unsigned short int half;
    dbg_printf("sh r%d, %d(r%d)\n", rt, imm & 0xFFFF, rs);
    half = RB[rt] & 0xFFFF;
    DATA_PORT->write_half(RB[rs] + imm, half);
    dbg_printf("Result = %#x\n", (int) half);

    insert_inst_pipeline(createInst(Itype, rs, rt, -3));
};

//!Instruction sw behavior method.
void ac_behavior( sw )
{
    dowrite(RB[rs]+imm, sizeof(int), DL1);
    dbg_printf("sw r%d, %d(r%d)\n", rt, imm & 0xFFFF, rs);
    DATA_PORT->write(RB[rs] + imm, RB[rt]);
    dbg_printf("Result = %#x\n", RB[rt]);

    insert_inst_pipeline(createInst(Itype, rs, rt, -3));
};

//!Instruction swl behavior method.
void ac_behavior( swl )
{
    dowrite((RB[rs]+imm) & 0xFFFFFFFC, sizeof(int), DL1);
    dbg_printf("swl r%d, %d(r%d)\n", rt, imm & 0xFFFF, rs);
    unsigned int addr, offset;
    ac_Uword data;

    addr = RB[rs] + imm;
    offset = (addr & 0x3) * 8;
    data = RB[rt];
    data >>= offset;
    data |= DATA_PORT->read(addr & 0xFFFFFFFC) & (0xFFFFFFFF << (32-offset));
    DATA_PORT->write(addr & 0xFFFFFFFC, data);
    dbg_printf("Result = %#x\n", data);

    insert_inst_pipeline(createInst(Itype, rs, rt, -3));
};

//!Instruction swr behavior method.
void ac_behavior( swr )
{
    dowrite((RB[rs]+imm) & 0xFFFFFFFC, sizeof(int), DL1);
    dbg_printf("swr r%d, %d(r%d)\n", rt, imm & 0xFFFF, rs);
    unsigned int addr, offset;
    ac_Uword data;

    addr = RB[rs] + imm;
    offset = (3 - (addr & 0x3)) * 8;
    data = RB[rt];
    data <<= offset;
    data |= DATA_PORT->read(addr & 0xFFFFFFFC) & ((1<<offset)-1);
    DATA_PORT->write(addr & 0xFFFFFFFC, data);
    dbg_printf("Result = %#x\n", data);

    insert_inst_pipeline(createInst(Itype, rs, rt, -3));
};

//!Instruction addi behavior method.
void ac_behavior( addi )
{
    dbg_printf("addi r%d, r%d, %d\n", rt, rs, imm & 0xFFFF);
    RB[rt] = RB[rs] + imm;
    dbg_printf("Result = %#x\n", RB[rt]);
    //Test overflow
    if ( ((RB[rs] & 0x80000000) == (imm & 0x80000000)) &&
            ((imm & 0x80000000) != (RB[rt] & 0x80000000)) ) {
        fprintf(stderr, "EXCEPTION(addi): integer overflow.\n"); exit(EXIT_FAILURE);
    }

    insert_inst_pipeline(createInst(Itype, rs, -2, rt));
};

//!Instruction addiu behavior method.
void ac_behavior( addiu )
{
    dbg_printf("addiu r%d, r%d, %d\n", rt, rs, imm & 0xFFFF);
    RB[rt] = RB[rs] + imm;
    dbg_printf("Result = %#x\n", RB[rt]);

    insert_inst_pipeline(createInst(Itype, rs, -2, rt));
};

//!Instruction slti behavior method.
void ac_behavior( slti )
{
    dbg_printf("slti r%d, r%d, %d\n", rt, rs, imm & 0xFFFF);
    // Set the RD if RS< IMM
    if( (ac_Sword) RB[rs] < (ac_Sword) imm )
        RB[rt] = 1;
    // Else reset RD
    else
        RB[rt] = 0;
    dbg_printf("Result = %#x\n", RB[rt]);

    insert_inst_pipeline(createInst(Itype, rs, -2, rt));
};

//!Instruction sltiu behavior method.
void ac_behavior( sltiu )
{
    dbg_printf("sltiu r%d, r%d, %d\n", rt, rs, imm & 0xFFFF);
    // Set the RD if RS< IMM
    if( (ac_Uword) RB[rs] < (ac_Uword) imm )
        RB[rt] = 1;
    // Else reset RD
    else
        RB[rt] = 0;
    dbg_printf("Result = %#x\n", RB[rt]);

    insert_inst_pipeline(createInst(Itype, rs, -2, rt));
};

//!Instruction andi behavior method.
void ac_behavior( andi )
{
    dbg_printf("andi r%d, r%d, %d\n", rt, rs, imm & 0xFFFF);
    RB[rt] = RB[rs] & (imm & 0xFFFF) ;
    dbg_printf("Result = %#x\n", RB[rt]);

    insert_inst_pipeline(createInst(Itype, rs, -2, rt));
};

//!Instruction ori behavior method.
void ac_behavior( ori )
{
    dbg_printf("ori r%d, r%d, %d\n", rt, rs, imm & 0xFFFF);
    RB[rt] = RB[rs] | (imm & 0xFFFF) ;
    dbg_printf("Result = %#x\n", RB[rt]);

    insert_inst_pipeline(createInst(Itype, rs, -2, rt));
};

//!Instruction xori behavior method.
void ac_behavior( xori )
{
    dbg_printf("xori r%d, r%d, %d\n", rt, rs, imm & 0xFFFF);
    RB[rt] = RB[rs] ^ (imm & 0xFFFF) ;
    dbg_printf("Result = %#x\n", RB[rt]);

    insert_inst_pipeline(createInst(Itype, rs, -2, rt));
};

//!Instruction lui behavior method.
void ac_behavior( lui )
{
    dbg_printf("lui r%d, r%d, %d\n", rt, rs, imm & 0xFFFF);
    // Load a constant in the upper 16 bits of a register
    // To achieve the desired behaviour, the constant was shifted 16 bits left
    // and moved to the target register ( rt )
    RB[rt] = imm << 16;
    dbg_printf("Result = %#x\n", RB[rt]);

    insert_inst_pipeline(createInst(Itype, rs, -2, rt));
};

//!Instruction add behavior method.
void ac_behavior( add )
{
    dbg_printf("add r%d, r%d, r%d\n", rd, rs, rt);
    RB[rd] = RB[rs] + RB[rt];
    dbg_printf("Result = %#x\n", RB[rd]);
    //Test overflow
    if ( ((RB[rs] & 0x80000000) == (RB[rd] & 0x80000000)) &&
            ((RB[rd] & 0x80000000) != (RB[rt] & 0x80000000)) ) {
        fprintf(stderr, "EXCEPTION(add): integer overflow.\n"); exit(EXIT_FAILURE);
    }

    insert_inst_pipeline(createInst(Rtype, rs, rt, rd));
};

//!Instruction addu behavior method.
void ac_behavior( addu )
{
    dbg_printf("addu r%d, r%d, r%d\n", rd, rs, rt);
    RB[rd] = RB[rs] + RB[rt];
    //cout << "  RS: " << (unsigned int)RB[rs] << " RT: " << (unsigned int)RB[rt] << endl;
    //cout << "  Result =  " <<  (unsigned int)RB[rd] <<endl;
    dbg_printf("Result = %#x\n", RB[rd]);

    insert_inst_pipeline(createInst(Rtype, rs, rt, rd));
};

//!Instruction sub behavior method.
void ac_behavior( sub )
{
    dbg_printf("sub r%d, r%d, r%d\n", rd, rs, rt);
    RB[rd] = RB[rs] - RB[rt];
    dbg_printf("Result = %#x\n", RB[rd]);
    //TODO: test integer overflow exception for sub

    insert_inst_pipeline(createInst(Rtype, rs, rt, rd));
};

//!Instruction subu behavior method.
void ac_behavior( subu )
{
    dbg_printf("subu r%d, r%d, r%d\n", rd, rs, rt);
    RB[rd] = RB[rs] - RB[rt];
    dbg_printf("Result = %#x\n", RB[rd]);

    insert_inst_pipeline(createInst(Rtype, rs, rt, rd));
};

//!Instruction slt behavior method.
void ac_behavior( slt )
{
    dbg_printf("slt r%d, r%d, r%d\n", rd, rs, rt);
    // Set the RD if RS< RT
    if( (ac_Sword) RB[rs] < (ac_Sword) RB[rt] )
        RB[rd] = 1;
    // Else reset RD
    else
        RB[rd] = 0;
    dbg_printf("Result = %#x\n", RB[rd]);

    insert_inst_pipeline(createInst(Rtype, rs, rt, rd));
};

//!Instruction sltu behavior method.
void ac_behavior( sltu )
{
    dbg_printf("sltu r%d, r%d, r%d\n", rd, rs, rt);
    // Set the RD if RS < RT
    if( RB[rs] < RB[rt] )
        RB[rd] = 1;
    // Else reset RD
    else
        RB[rd] = 0;
    dbg_printf("Result = %#x\n", RB[rd]);

    insert_inst_pipeline(createInst(Rtype, rs, rt, rd));
};

//!Instruction instr_and behavior method.
void ac_behavior( instr_and )
{
    dbg_printf("instr_and r%d, r%d, r%d\n", rd, rs, rt);
    RB[rd] = RB[rs] & RB[rt];
    dbg_printf("Result = %#x\n", RB[rd]);

    insert_inst_pipeline(createInst(Rtype, rs, rt, rd));
};

//!Instruction instr_or behavior method.
void ac_behavior( instr_or )
{
    dbg_printf("instr_or r%d, r%d, r%d\n", rd, rs, rt);
    RB[rd] = RB[rs] | RB[rt];
    dbg_printf("Result = %#x\n", RB[rd]);

    insert_inst_pipeline(createInst(Rtype, rs, rt, rd));
};

//!Instruction instr_xor behavior method.
void ac_behavior( instr_xor )
{
    dbg_printf("instr_xor r%d, r%d, r%d\n", rd, rs, rt);
    RB[rd] = RB[rs] ^ RB[rt];
    dbg_printf("Result = %#x\n", RB[rd]);

    insert_inst_pipeline(createInst(Rtype, rs, rt, rd));
};

//!Instruction instr_nor behavior method.
void ac_behavior( instr_nor )
{
    dbg_printf("nor r%d, r%d, r%d\n", rd, rs, rt);
    RB[rd] = ~(RB[rs] | RB[rt]);
    dbg_printf("Result = %#x\n", RB[rd]);

    insert_inst_pipeline(createInst(Rtype, rs, rt, rd));
};

//!Instruction nop behavior method.
void ac_behavior( nop )
{
    dbg_printf("nop\n");

    insert_inst_pipeline(createInst(Rtype, -1, -2, -3));
};

//!Instruction sll behavior method.
void ac_behavior( sll )
{
    dbg_printf("sll r%d, r%d, %d\n", rd, rs, shamt);
    RB[rd] = RB[rt] << shamt;
    dbg_printf("Result = %#x\n", RB[rd]);

    insert_inst_pipeline(createInst(Rtype, rs, rt, rd));
};

//!Instruction srl behavior method.
void ac_behavior( srl )
{
    dbg_printf("srl r%d, r%d, %d\n", rd, rs, shamt);
    RB[rd] = RB[rt] >> shamt;
    dbg_printf("Result = %#x\n", RB[rd]);

    insert_inst_pipeline(createInst(Rtype, rs, rt, rd));
};

//!Instruction sra behavior method.
void ac_behavior( sra )
{
    dbg_printf("sra r%d, r%d, %d\n", rd, rs, shamt);
    RB[rd] = (ac_Sword) RB[rt] >> shamt;
    dbg_printf("Result = %#x\n", RB[rd]);

    insert_inst_pipeline(createInst(Rtype, rs, rt, rd));
};

//!Instruction sllv behavior method.
void ac_behavior( sllv )
{
    dbg_printf("sllv r%d, r%d, r%d\n", rd, rt, rs);
    RB[rd] = RB[rt] << (RB[rs] & 0x1F);
    dbg_printf("Result = %#x\n", RB[rd]);

    insert_inst_pipeline(createInst(Rtype, rs, rt, rd));
};

//!Instruction srlv behavior method.
void ac_behavior( srlv )
{
    dbg_printf("srlv r%d, r%d, r%d\n", rd, rt, rs);
    RB[rd] = RB[rt] >> (RB[rs] & 0x1F);
    dbg_printf("Result = %#x\n", RB[rd]);

    insert_inst_pipeline(createInst(Rtype, rs, rt, rd));
};

//!Instruction srav behavior method.
void ac_behavior( srav )
{
    dbg_printf("srav r%d, r%d, r%d\n", rd, rt, rs);
    RB[rd] = (ac_Sword) RB[rt] >> (RB[rs] & 0x1F);
    dbg_printf("Result = %#x\n", RB[rd]);

    insert_inst_pipeline(createInst(Rtype, rs, rt, rd));
};

//!Instruction mult behavior method.
void ac_behavior( mult )
{
    dbg_printf("mult r%d, r%d\n", rs, rt);

    long long result;
    int half_result;

    result = (ac_Sword) RB[rs];
    result *= (ac_Sword) RB[rt];

    half_result = (result & 0xFFFFFFFF);
    // Register LO receives 32 less significant bits
    lo = half_result;

    half_result = ((result >> 32) & 0xFFFFFFFF);
    // Register HI receives 32 most significant bits
    hi = half_result ;

    dbg_printf("Result = %#llx\n", result);

    insert_inst_pipeline(createInst(Rtype, rs, rt, -3));
};

//!Instruction multu behavior method.
void ac_behavior( multu )
{
    dbg_printf("multu r%d, r%d\n", rs, rt);

    unsigned long long result;
    unsigned int half_result;

    result  = RB[rs];
    result *= RB[rt];

    half_result = (result & 0xFFFFFFFF);
    // Register LO receives 32 less significant bits
    lo = half_result;

    half_result = ((result>>32) & 0xFFFFFFFF);
    // Register HI receives 32 most significant bits
    hi = half_result ;

    dbg_printf("Result = %#llx\n", result);

    insert_inst_pipeline(createInst(Rtype, rs, rt, -3));
};

//!Instruction div behavior method.
void ac_behavior( div )
{
    dbg_printf("div r%d, r%d\n", rs, rt);
    // Register LO receives quotient
    lo = (ac_Sword) RB[rs] / (ac_Sword) RB[rt];
    // Register HI receives remainder
    hi = (ac_Sword) RB[rs] % (ac_Sword) RB[rt];

    insert_inst_pipeline(createInst(Rtype, rs, rt, -3));
};

//!Instruction divu behavior method.
void ac_behavior( divu )
{
    dbg_printf("divu r%d, r%d\n", rs, rt);
    // Register LO receives quotient
    lo = RB[rs] / RB[rt];
    // Register HI receives remainder
    hi = RB[rs] % RB[rt];

    insert_inst_pipeline(createInst(Rtype, rs, rt, -3));
};

//!Instruction mfhi behavior method.
void ac_behavior( mfhi )
{
    dbg_printf("mfhi r%d\n", rd);
    RB[rd] = hi;
    dbg_printf("Result = %#x\n", RB[rd]);

    insert_inst_pipeline(createInst(Rtype, -1, -2, rd));
};

//!Instruction mthi behavior method.
void ac_behavior( mthi )
{
    dbg_printf("mthi r%d\n", rs);
    hi = RB[rs];
    dbg_printf("Result = %#x\n", (unsigned int) hi);

    insert_inst_pipeline(createInst(Rtype, rs, -2, -3));
};

//!Instruction mflo behavior method.
void ac_behavior( mflo )
{
    dbg_printf("mflo r%d\n", rd);
    RB[rd] = lo;
    dbg_printf("Result = %#x\n", RB[rd]);

    insert_inst_pipeline(createInst(Rtype, -1, -2, rd));
};

//!Instruction mtlo behavior method.
void ac_behavior( mtlo )
{
    dbg_printf("mtlo r%d\n", rs);
    lo = RB[rs];
    dbg_printf("Result = %#x\n", (unsigned int) lo);

    insert_inst_pipeline(createInst(Rtype, rs, -2, -3));
};

//!Instruction j behavior method.
void ac_behavior( j )
{
    dbg_printf("j %d\n", addr);
    addr = addr << 2;
#ifndef NO_NEED_PC_UPDATE
    npc =  (ac_pc & 0xF0000000) | addr;
#endif
    dbg_printf("Target = %#x\n", (ac_pc & 0xF0000000) | addr );

    insert_inst_pipeline(createInst(Jtype, -1, -2, -3));
};

//!Instruction jal behavior method.
void ac_behavior( jal )
{
    dbg_printf("jal %d\n", addr);
    // Save the value of PC + 8 (return address) in $ra ($31) and
    // jump to the address given by PC(31...28)||(addr<<2)
    // It must also flush the instructions that were loaded into the pipeline
    RB[Ra] = ac_pc+4; //ac_pc is pc+4, we need pc+8

    addr = addr << 2;
#ifndef NO_NEED_PC_UPDATE
    npc = (ac_pc & 0xF0000000) | addr;
#endif

    dbg_printf("Target = %#x\n", (ac_pc & 0xF0000000) | addr );
    dbg_printf("Return = %#x\n", ac_pc+4);

    insert_inst_pipeline(createInst(Jtype, -1, -2, Ra));
};

//!Instruction jr behavior method.
void ac_behavior( jr )
{
    dbg_printf("jr r%d\n", rs);
    // Jump to the address stored on the register reg[RS]
    // It must also flush the instructions that were loaded into the pipeline
#ifndef NO_NEED_PC_UPDATE
    npc = RB[rs], 1;
#endif
    dbg_printf("Target = %#x\n", RB[rs]);

    insert_inst_pipeline(createInst(Jtype, rs, -2, -3));
};

//!Instruction jalr behavior method.
void ac_behavior( jalr )
{
    dbg_printf("jalr r%d, r%d\n", rd, rs);
    // Save the value of PC + 8(return address) in rd and
    // jump to the address given by [rs]

#ifndef NO_NEED_PC_UPDATE
    npc = RB[rs], 1;
#endif
    dbg_printf("Target = %#x\n", RB[rs]);

    if( rd == 0 )  //If rd is not defined use default
        rd = Ra;
    RB[rd] = ac_pc+4;
    dbg_printf("Return = %#x\n", ac_pc+4);

    insert_inst_pipeline(createInst(Jtype, rs, -2, rd));
};

//!Instruction beq behavior method.
void ac_behavior( beq )
{
    bool branch_taken = false;

    dbg_printf("beq r%d, r%d, %d\n", rt, rs, imm & 0xFFFF);
    if( RB[rs] == RB[rt] ){
#ifndef NO_NEED_PC_UPDATE
        npc = ac_pc + (imm<<2);
#endif
        dbg_printf("Taken to %#x\n", ac_pc + (imm<<2));
        branch_taken = true;
    }

    insert_inst_pipeline(createInst(Itype, rs, rt, -3));

    if (branch_taken){
        branch_taken_pipeline();
    } else {
        branch_not_taken_pipeline();
    }
};

//!Instruction bne behavior method.
void ac_behavior( bne )
{
    bool branch_taken = false;

    dbg_printf("bne r%d, r%d, %d\n", rt, rs, imm & 0xFFFF);
    if( RB[rs] != RB[rt] ){
#ifndef NO_NEED_PC_UPDATE
        npc = ac_pc + (imm<<2);
#endif
        dbg_printf("Taken to %#x\n", ac_pc + (imm<<2));
        branch_taken = true;
    }

    insert_inst_pipeline(createInst(Itype, rs, rt, -3));

    if (branch_taken){
        branch_taken_pipeline();
    } else {
        branch_not_taken_pipeline();
    }
};

//!Instruction blez behavior method.
void ac_behavior( blez )
{
    bool branch_taken = false;

    dbg_printf("blez r%d, %d\n", rs, imm & 0xFFFF);
    if( (RB[rs] == 0 ) || (RB[rs]&0x80000000 ) ){
#ifndef NO_NEED_PC_UPDATE
        npc = ac_pc + (imm<<2), 1;
#endif
        dbg_printf("Taken to %#x\n", ac_pc + (imm<<2));
        branch_taken = true;
    }

    insert_inst_pipeline(createInst(Itype, rs, -2, -3));

    if (branch_taken){
        branch_taken_pipeline();
    } else {
        branch_not_taken_pipeline();
    }
};

//!Instruction bgtz behavior method.
void ac_behavior( bgtz )
{
    bool branch_taken = false;

    dbg_printf("bgtz r%d, %d\n", rs, imm & 0xFFFF);
    if( !(RB[rs] & 0x80000000) && (RB[rs]!=0) ){
#ifndef NO_NEED_PC_UPDATE
        npc = ac_pc + (imm<<2);
#endif
        dbg_printf("Taken to %#x\n", ac_pc + (imm<<2));
        branch_taken = true;
    }

    insert_inst_pipeline(createInst(Itype, rs, -2, -3));


    if (branch_taken){
        branch_taken_pipeline();
    } else {
        branch_not_taken_pipeline();
    }
};

//!Instruction bltz behavior method.
void ac_behavior( bltz )
{
    bool branch_taken = false;

    dbg_printf("bltz r%d, %d\n", rs, imm & 0xFFFF);
    if( RB[rs] & 0x80000000 ){
#ifndef NO_NEED_PC_UPDATE
        npc = ac_pc + (imm<<2);
#endif
        dbg_printf("Taken to %#x\n", ac_pc + (imm<<2));
        branch_taken = true;
    }

    insert_inst_pipeline(createInst(Itype, rs, -2, -3));

    if (branch_taken){
        branch_taken_pipeline();
    } else {
        branch_not_taken_pipeline();
    }
};

//!Instruction bgez behavior method.
void ac_behavior( bgez )
{
    bool branch_taken = false;

    dbg_printf("bgez r%d, %d\n", rs, imm & 0xFFFF);
    if( !(RB[rs] & 0x80000000) ){
#ifndef NO_NEED_PC_UPDATE
        npc = ac_pc + (imm<<2);
#endif
        dbg_printf("Taken to %#x\n", ac_pc + (imm<<2));
        branch_taken = true;
    }

    insert_inst_pipeline(createInst(Itype, rs, -2, -3));

    if (branch_taken){
        branch_taken_pipeline();
    } else {
        branch_not_taken_pipeline();
    }
};

//!Instruction bltzal behavior method.
void ac_behavior( bltzal )
{
    bool branch_taken = false;

    dbg_printf("bltzal r%d, %d\n", rs, imm & 0xFFFF);
    RB[Ra] = ac_pc+4; //ac_pc is pc+4, we need pc+8
    if( RB[rs] & 0x80000000 ){
#ifndef NO_NEED_PC_UPDATE
        npc = ac_pc + (imm<<2);
#endif
        dbg_printf("Taken to %#x\n", ac_pc + (imm<<2));
        branch_taken = true;
    }
    dbg_printf("Return = %#x\n", ac_pc+4);

    insert_inst_pipeline(createInst(Itype, rs, -2, Ra));

    if (branch_taken){
        branch_taken_pipeline();
    } else {
        branch_not_taken_pipeline();
    }
};

//!Instruction bgezal behavior method.
void ac_behavior( bgezal )
{
    bool branch_taken = false;

    dbg_printf("bgezal r%d, %d\n", rs, imm & 0xFFFF);
    RB[Ra] = ac_pc+4; //ac_pc is pc+4, we need pc+8
    if( !(RB[rs] & 0x80000000) ){
#ifndef NO_NEED_PC_UPDATE
        npc = ac_pc + (imm<<2);
#endif
        dbg_printf("Taken to %#x\n", ac_pc + (imm<<2));
        branch_taken = true;
    }
    dbg_printf("Return = %#x\n", ac_pc+4);

    insert_inst_pipeline(createInst(Itype, rs, -2, Ra));

    if (branch_taken){
        branch_taken_pipeline();
    } else {
        branch_not_taken_pipeline();
    }
};

//!Instruction sys_call behavior method.
void ac_behavior( sys_call )
{

    insert_inst_pipeline(createInst(Itype, -1, -2, -3));

    dbg_printf("syscall\n");
    stop();
}

//!Instruction instr_break behavior method.
void ac_behavior( instr_break )
{
    fprintf(stderr, "instr_break behavior not implemented.\n");
    exit(EXIT_FAILURE);
}
