# 🔄 Minishell Modular Diagrams for Excalidraw

**Project**: Minishell  
**Last Updated**: November 5, 2025  
**Purpose**: Individual component diagrams optimized for Excalidraw import

---

## 📋 Table of Contents

1. [Initialization Phase](#1-initialization-phase)
2. [Main Loop (REPL)](#2-main-loop-repl)
3. [Lexer - Tokenization](#3-lexer---tokenization)
4. [Parser - AST Building](#4-parser---ast-building)
5. [Expander - Variable Expansion](#5-expander---variable-expansion)
6. [Executor - Built-in Commands](#6-executor---built-in-commands)
7. [Executor - External Commands](#7-executor---external-commands)
8. [Executor - Pipeline](#8-executor---pipeline)
9. [Redirections](#9-redirections)
10. [Signal Handling](#10-signal-handling)
11. [Memory Management](#11-memory-management)

---

## 1. Initialization Phase

```mermaid
flowchart TD
    Start([Start Minishell]) --> ParseEnv[Parse envp array]
    ParseEnv --> CreateList[Create t_env linked list]
    CreateList --> CheckSuccess{Success?}
    CheckSuccess -->|No| ExitError[Exit with error]
    CheckSuccess -->|Yes| SetupSignals[Setup Signal Handlers]
    SetupSignals --> ConfigSIGINT[SIGINT: Display new prompt]
    SetupSignals --> ConfigSIGQUIT[SIGQUIT: Ignore]
    ConfigSIGINT --> IncrSHLVL[Increment SHLVL]
    ConfigSIGQUIT --> IncrSHLVL
    IncrSHLVL --> InitState[Initialize Shell State]
    InitState --> SetExitStatus[last_exit_status = 0]
    SetExitStatus --> EnterLoop[Enter Main Loop]
    
    style Start fill:#90EE90
    style ExitError fill:#FFB6C1
    style EnterLoop fill:#87CEEB
```

---

## 2. Main Loop (REPL)

```mermaid
flowchart TD
    Loop([Main Loop]) --> DisplayPrompt[Display: minishell$]
    DisplayPrompt --> ReadLine[readline input]
    ReadLine --> CheckEOF{EOF Ctrl+D?}
    CheckEOF -->|Yes| Cleanup[Cleanup & Exit]
    CheckEOF -->|No| CheckEmpty{Empty input?}
    CheckEmpty -->|Yes| Loop
    CheckEmpty -->|No| AddHistory[add_history line]
    AddHistory --> Tokenize[Call Lexer]
    Tokenize --> Parse[Call Parser]
    Parse --> Expand[Call Expander]
    Expand --> Execute[Call Executor]
    Execute --> UpdateStatus[Update exit status]
    UpdateStatus --> FreeMemory[Free tokens, AST, line]
    FreeMemory --> Loop
    Cleanup --> ExitShell([Exit Shell])
    
    style Loop fill:#87CEEB
    style Tokenize fill:#FFD700
    style Parse fill:#FFA500
    style Expand fill:#DDA0DD
    style Execute fill:#98FB98
    style ExitShell fill:#90EE90
```

---

## 3. Lexer - Tokenization

```mermaid
flowchart TD
    Input[Input String] --> InitList[Initialize token list]
    InitList --> ScanChar{Scan character}
    ScanChar --> CheckQuote{Quote ' or ?}
    CheckQuote -->|Yes| FindClose[Find closing quote]
    FindClose --> QuoteClosed{Closed?}
    QuoteClosed -->|No| SyntaxErr[Syntax Error]
    QuoteClosed -->|Yes| CreateWord1[Create TOKEN_WORD]
    CreateWord1 --> ScanChar
    
    CheckQuote -->|No| CheckOp{Operator?}
    CheckOp -->|Yes| IdentifyOp[Identify operator type]
    IdentifyOp --> CheckPipe{Pipe ?}
    CheckPipe -->|Yes| CreatePipe[Create TOKEN_PIPE]
    CheckPipe -->|No| CheckRedir{Redir < > << >>?}
    CheckRedir -->|Yes| CreateRedir[Create TOKEN_REDIR_*]
    CheckRedir -->|No| CheckLogical{&& or or?}
    CheckLogical -->|Yes| CreateLogical[Create TOKEN_AND/OR]
    CreatePipe --> ScanChar
    CreateRedir --> ScanChar
    CreateLogical --> ScanChar
    
    CheckOp -->|No| CheckWord{Word char?}
    CheckWord -->|Yes| ExtractWord[Extract word]
    ExtractWord --> CreateWord2[Create TOKEN_WORD]
    CreateWord2 --> ScanChar
    CheckWord -->|No| SkipSpace[Skip whitespace]
    SkipSpace --> ScanChar
    
    ScanChar --> EndInput{End of input?}
    EndInput -->|No| ScanChar
    EndInput -->|Yes| AddEOF[Add TOKEN_EOF]
    AddEOF --> ReturnList[Return token list]
    SyntaxErr --> ReturnNull[Return NULL]
    
    style Input fill:#FFD700
    style ReturnList fill:#FFD700
    style SyntaxErr fill:#FFB6C1
```

---

## 4. Parser - AST Building

```mermaid
flowchart TD
    Tokens[Token List] --> Validate{Validate syntax}
    Validate -->|Invalid| SyntaxError[Return syntax error]
    Validate -->|Valid| ParseLogical[Parse logical operators]
    ParseLogical --> CheckAnd{Found && or ?}
    CheckAnd -->|Yes| CreateLogNode[Create AND/OR node]
    CreateLogNode --> ParseLeft1[Parse left subtree]
    ParseLeft1 --> ParseRight1[Parse right subtree]
    ParseRight1 --> ParsePipes
    CheckAnd -->|No| ParsePipes[Parse pipes]
    
    ParsePipes --> CheckPipe{Found pipe ?}
    CheckPipe -->|Yes| CreatePipeNode[Create PIPE node]
    CreatePipeNode --> ParseLeft2[Parse left command]
    ParseLeft2 --> ParseRight2[Parse right pipeline]
    ParseRight2 --> ParseCmd
    CheckPipe -->|No| ParseCmd[Parse simple command]
    
    ParseCmd --> ExtractCmd[Extract command name]
    ExtractCmd --> ParseRedir{Redirections?}
    ParseRedir -->|Yes| CreateRedirList[Create redir list]
    CreateRedirList --> CheckFile{Next is file?}
    CheckFile -->|No| RedirError[Syntax error]
    CheckFile -->|Yes| AddToList[Add to redir list]
    AddToList --> ParseRedir
    ParseRedir -->|No| CollectArgs[Collect arguments]
    CollectArgs --> BuildNode[Build AST node]
    BuildNode --> AttachRedir[Attach redirections]
    AttachRedir --> ReturnAST[Return AST]
    
    SyntaxError --> ReturnNull[Return NULL]
    RedirError --> ReturnNull
    
    style Tokens fill:#FFA500
    style ReturnAST fill:#FFA500
    style SyntaxError fill:#FFB6C1
    style RedirError fill:#FFB6C1
```

---

## 5. Expander - Variable Expansion

```mermaid
flowchart TD
    AST[AST Tree] --> TraverseNodes[Traverse AST nodes]
    TraverseNodes --> ScanArgs[Scan arguments]
    ScanArgs --> FindDollar{Found $?}
    FindDollar -->|No| RemoveQuotes[Remove quotes]
    FindDollar -->|Yes| CheckSpecial{$? special?}
    
    CheckSpecial -->|Yes| GetStatus[Get last_exit_status]
    GetStatus --> ConvertStr[Convert to string]
    ConvertStr --> ReplaceStatus[Replace $?]
    ReplaceStatus --> ScanArgs
    
    CheckSpecial -->|No| CheckContext{In single quote?}
    CheckContext -->|Yes| KeepLiteral[Keep literal $VAR]
    KeepLiteral --> ScanArgs
    
    CheckContext -->|No| ExtractName[Extract variable name]
    ExtractName --> LookupEnv[Lookup in environment]
    LookupEnv --> CheckExists{Exists?}
    CheckExists -->|No| ReplaceEmpty[Replace with empty]
    CheckExists -->|Yes| GetValue[Get value]
    GetValue --> ReplaceVar[Replace $VAR]
    ReplaceEmpty --> ScanArgs
    ReplaceVar --> ScanArgs
    
    RemoveQuotes --> StripQuotes[Strip quote characters]
    StripQuotes --> ReturnExpanded[Return expanded AST]
    
    style AST fill:#DDA0DD
    style ReturnExpanded fill:#DDA0DD
```

---

## 6. Executor - Built-in Commands

```mermaid
flowchart TD
    CheckBuiltin{Is built-in?} --> CheckType{Which command?}
    
    CheckType -->|echo| ExecEcho[Execute echo]
    CheckType -->|cd| ExecCD[Execute cd]
    CheckType -->|pwd| ExecPWD[Execute pwd]
    CheckType -->|export| ExecExport[Execute export]
    CheckType -->|unset| ExecUnset[Execute unset]
    CheckType -->|env| ExecEnv[Execute env]
    CheckType -->|exit| ExecExit[Execute exit]
    
    ExecEcho --> CheckN{Has -n flag?}
    CheckN -->|Yes| PrintNoNL[Print without newline]
    CheckN -->|No| PrintNL[Print with newline]
    PrintNoNL --> Return0_1[Return 0]
    PrintNL --> Return0_1
    
    ExecCD --> GetTarget[Get target directory]
    GetTarget --> CallChdir[chdir target]
    CallChdir --> CheckSuccess1{Success?}
    CheckSuccess1 -->|Yes| UpdatePWD[Update PWD/OLDPWD]
    CheckSuccess1 -->|No| Return1[Return 1]
    UpdatePWD --> Return0_2[Return 0]
    
    ExecPWD --> GetCWD[getcwd]
    GetCWD --> PrintPath[Print path]
    PrintPath --> Return0_3[Return 0]
    
    ExecExport --> CheckArgs{Has args?}
    CheckArgs -->|No| PrintAll[Print all vars sorted]
    CheckArgs -->|Yes| ParseVar[Parse NAME=value]
    ParseVar --> ValidateName{Valid name?}
    ValidateName -->|No| Return1_2[Return 1]
    ValidateName -->|Yes| SetEnv[Set in environment]
    SetEnv --> Return0_4[Return 0]
    PrintAll --> Return0_4
    
    ExecUnset --> GetVarName[Get variable name]
    GetVarName --> RemoveEnv[Remove from environment]
    RemoveEnv --> Return0_5[Return 0]
    
    ExecEnv --> IterateEnv[Iterate environment list]
    IterateEnv --> PrintVar[Print NAME=value]
    PrintVar --> Return0_6[Return 0]
    
    ExecExit --> CheckExitArgs{Has args?}
    CheckExitArgs -->|No| ExitLast[Exit with last status]
    CheckExitArgs -->|Yes| CheckNumeric{Numeric?}
    CheckNumeric -->|No| ExitError[Print error, exit 2]
    CheckNumeric -->|Yes| CheckCount{Multiple args?}
    CheckCount -->|Yes| ErrorReturn[Error: too many args]
    CheckCount -->|No| ExitNum[Exit with arg % 256]
    
    Return0_1 --> UpdateStatus1[Update exit status]
    Return1 --> UpdateStatus1
    Return0_2 --> UpdateStatus1
    Return0_3 --> UpdateStatus1
    Return0_4 --> UpdateStatus1
    Return1_2 --> UpdateStatus1
    Return0_5 --> UpdateStatus1
    Return0_6 --> UpdateStatus1
    ErrorReturn --> UpdateStatus1
    
    style ExecEcho fill:#98FB98
    style ExecCD fill:#98FB98
    style ExecPWD fill:#98FB98
    style ExecExport fill:#98FB98
    style ExecUnset fill:#98FB98
    style ExecEnv fill:#98FB98
    style ExecExit fill:#98FB98
```

---

## 7. Executor - External Commands

```mermaid
flowchart TD
    ExtCmd[External Command] --> CheckPath{Has / in name?}
    CheckPath -->|Yes| UseAbsolute[Use as absolute path]
    CheckPath -->|No| GetPATH[Get PATH from env]
    GetPATH --> SplitPATH[Split by colon]
    SplitPATH --> SearchLoop{For each directory}
    SearchLoop --> BuildPath[Build full path]
    BuildPath --> CheckAccess{access X_OK?}
    CheckAccess -->|No| NextDir[Try next directory]
    CheckAccess -->|Yes| Found[Command found]
    NextDir --> SearchLoop
    SearchLoop --> NotFound{No more dirs?}
    NotFound -->|Yes| CmdNotFound[Command not found]
    
    UseAbsolute --> CheckAccess2{access X_OK?}
    CheckAccess2 -->|No| PermDenied[Permission denied]
    CheckAccess2 -->|Yes| Found
    
    Found --> ForkProc[fork]
    ForkProc --> CheckFork{Fork success?}
    CheckFork -->|No| ForkError[Fork error]
    CheckFork -->|Yes| InChild{In child?}
    
    InChild -->|Yes| SetupRedir[Setup redirections]
    SetupRedir --> ConvertEnv[Convert env to array]
    ConvertEnv --> CallExecve[execve path args env]
    CallExecve --> ExecFailed[execve failed]
    ExecFailed --> Exit126[exit 126]
    
    InChild -->|No| WaitPid[waitpid]
    WaitPid --> CollectStatus[Collect exit status]
    CollectStatus --> CheckType{Status type?}
    CheckType -->|Exited| GetExit[WEXITSTATUS]
    CheckType -->|Signaled| GetSig[128 + signal]
    GetExit --> ReturnStatus[Return status]
    GetSig --> ReturnStatus
    
    CmdNotFound --> Exit127[exit 127]
    PermDenied --> Exit126_2[exit 126]
    ForkError --> ReturnError[Return error]
    
    style ExtCmd fill:#98FB98
    style ReturnStatus fill:#98FB98
    style CmdNotFound fill:#FFB6C1
    style PermDenied fill:#FFB6C1
    style ForkError fill:#FFB6C1
```

---

## 8. Executor - Pipeline

```mermaid
flowchart TD
    Pipeline[Pipeline Node] --> CountCmds[Count commands]
    CountCmds --> CalcPipes[Calculate pipes needed<br/>n-1 for n commands]
    CalcPipes --> CreatePipes[Create all pipes]
    CreatePipes --> CheckPipeErr{Pipe error?}
    CheckPipeErr -->|Yes| PipeError[Return error]
    CheckPipeErr -->|No| InitLoop[Initialize loop]
    
    InitLoop --> ForkLoop{For each command}
    ForkLoop --> ForkChild[fork child]
    ForkChild --> CheckFork{Fork success?}
    CheckFork -->|No| ForkError[Fork error]
    CheckFork -->|Yes| InChild{In child?}
    
    InChild -->|Yes| CheckPos{Command position?}
    CheckPos -->|First| SetupFirst[dup2 pipe write to stdout]
    CheckPos -->|Middle| SetupMiddle[dup2 pipe read to stdin<br/>dup2 pipe write to stdout]
    CheckPos -->|Last| SetupLast[dup2 pipe read to stdin]
    
    SetupFirst --> CloseAll[Close all pipe FDs]
    SetupMiddle --> CloseAll
    SetupLast --> CloseAll
    CloseAll --> ExecCmd[Execute command]
    ExecCmd --> ExitChild[exit with status]
    
    InChild -->|No| StorePID[Store child PID]
    StorePID --> MoreCmds{More commands?}
    MoreCmds -->|Yes| ForkLoop
    MoreCmds -->|No| CloseParent[Parent: Close all pipes]
    
    CloseParent --> WaitLoop[Wait for all children]
    WaitLoop --> WaitNext{More children?}
    WaitNext -->|Yes| WaitPid[waitpid next child]
    WaitPid --> WaitNext
    WaitNext -->|No| GetLast[Get last command status]
    GetLast --> ReturnStatus[Return status]
    
    PipeError --> ReturnError[Return error]
    ForkError --> ReturnError
    
    style Pipeline fill:#98FB98
    style ReturnStatus fill:#98FB98
    style PipeError fill:#FFB6C1
    style ForkError fill:#FFB6C1
```

---

## 9. Redirections

```mermaid
flowchart TD
    Redir[Redirection Node] --> CheckType{Redirection type?}
    
    CheckType -->|Input <| OpenInput[open O_RDONLY]
    OpenInput --> CheckErr1{Open error?}
    CheckErr1 -->|Yes| FileNotFound[File not found]
    CheckErr1 -->|No| Dup2Input[dup2 fd STDIN]
    Dup2Input --> CloseInput[close fd]
    CloseInput --> Success1[Success]
    
    CheckType -->|Output >| OpenOutput[open O_WRONLY|O_CREAT|O_TRUNC]
    OpenOutput --> CheckErr2{Open error?}
    CheckErr2 -->|Yes| PermError1[Permission denied]
    CheckErr2 -->|No| Dup2Output[dup2 fd STDOUT]
    Dup2Output --> CloseOutput[close fd]
    CloseOutput --> Success2[Success]
    
    CheckType -->|Append >>| OpenAppend[open O_WRONLY|O_CREAT|O_APPEND]
    OpenAppend --> CheckErr3{Open error?}
    CheckErr3 -->|Yes| PermError2[Permission denied]
    CheckErr3 -->|No| Dup2Append[dup2 fd STDOUT]
    Dup2Append --> CloseAppend[close fd]
    CloseAppend --> Success3[Success]
    
    CheckType -->|Heredoc <<| CreatePipe[Create pipe]
    CreatePipe --> ReadLoop[Read lines until delimiter]
    ReadLoop --> CheckDelim{Line == delimiter?}
    CheckDelim -->|No| ExpandLine[Expand variables if needed]
    ExpandLine --> WritePipe[Write to pipe]
    WritePipe --> ReadLoop
    CheckDelim -->|Yes| CloseWrite[Close write end]
    CloseWrite --> Dup2Heredoc[dup2 read end to STDIN]
    Dup2Heredoc --> CloseRead[Close read end]
    CloseRead --> Success4[Success]
    
    Success1 --> Done[Return success]
    Success2 --> Done
    Success3 --> Done
    Success4 --> Done
    FileNotFound --> Error[Return error]
    PermError1 --> Error
    PermError2 --> Error
    
    style Redir fill:#98FB98
    style Done fill:#90EE90
    style Error fill:#FFB6C1
```

---

## 10. Signal Handling

```mermaid
flowchart TD
    Setup[Setup Signals] --> ConfigSIGINT[Configure SIGINT]
    ConfigSIGINT --> ConfigSIGQUIT[Configure SIGQUIT]
    ConfigSIGQUIT --> Ready[Signals Ready]
    
    SIGINT[SIGINT Received<br/>Ctrl+C] --> CheckContext{Context?}
    CheckContext -->|Interactive| NewLine[Write newline]
    NewLine --> RLNewLine[rl_on_new_line]
    RLNewLine --> ClearLine[rl_replace_line]
    ClearLine --> Redisplay[rl_redisplay]
    Redisplay --> Continue[Continue]
    
    CheckContext -->|Executing| ChildReceives[Child receives signal]
    ChildReceives --> ChildTerminates[Child terminates]
    ChildTerminates --> ParentWaits[Parent waits]
    ParentWaits --> GetSignal[Get signal number]
    GetSignal --> CalcStatus[status = 128 + signal]
    CalcStatus --> UpdateStatus[Update exit status]
    UpdateStatus --> BackPrompt[Back to prompt]
    
    SIGQUIT[SIGQUIT Received<br/>Ctrl+\\] --> CheckContext2{Context?}
    CheckContext2 -->|Interactive| Ignore[Ignore do nothing]
    CheckContext2 -->|Executing| ChildHandle[Child may terminate]
    
    EOF[EOF Received<br/>Ctrl+D] --> CheckLine{readline NULL?}
    CheckLine -->|Yes| PrintExit[Print exit]
    PrintExit --> Cleanup[Cleanup resources]
    Cleanup --> ExitShell[exit with status]
    
    style Setup fill:#F0E68C
    style SIGINT fill:#FFB6C1
    style SIGQUIT fill:#FFB6C1
    style EOF fill:#FFB6C1
    style ExitShell fill:#90EE90
```

---

## 11. Memory Management

```mermaid
flowchart TD
    Alloc[Memory Allocations] --> AllocEnv[Environment list<br/>malloc t_env]
    Alloc --> AllocLine[Input line<br/>readline]
    Alloc --> AllocTokens[Tokens<br/>malloc t_token]
    Alloc --> AllocAST[AST nodes<br/>malloc t_ast_node]
    Alloc --> AllocExpand[Expanded strings<br/>malloc during expansion]
    
    AllocEnv --> Track1[Track pointer]
    AllocLine --> Track1
    AllocTokens --> Track1
    AllocAST --> Track1
    AllocExpand --> Track1
    
    Track1 --> PerCmd[Per-Command Cleanup]
    PerCmd --> FreeLine[free line]
    FreeLine --> FreeTokens[Free token list]
    FreeTokens --> IterTokens[Iterate tokens]
    IterTokens --> FreeValue[free token->value]
    FreeValue --> FreeToken[free token]
    FreeToken --> NextToken{More tokens?}
    NextToken -->|Yes| IterTokens
    NextToken -->|No| FreeAST[Free AST tree]
    
    FreeAST --> PostOrder[Post-order traversal]
    PostOrder --> FreeLeft[Free left subtree]
    FreeLeft --> FreeRight[Free right subtree]
    FreeRight --> FreeArgs[Free args array]
    FreeArgs --> FreeRedirs[Free redirections]
    FreeRedirs --> FreeNode[Free node]
    FreeNode --> ReadyNext[Ready for next command]
    
    OnExit[On Exit] --> FreeEnvList[Free environment list]
    FreeEnvList --> IterEnv[Iterate env list]
    IterEnv --> FreeName[free env->name]
    FreeName --> FreeValue2[free env->value]
    FreeValue2 --> FreeEnvNode[free env node]
    FreeEnvNode --> NextEnv{More env nodes?}
    NextEnv -->|Yes| IterEnv
    NextEnv -->|No| ClearHist[rl_clear_history]
    ClearHist --> CloseFDs[Close file descriptors]
    CloseFDs --> ExitClean[Exit cleanly]
    
    ErrorPath[Error Path] --> PartialCleanup[Cleanup allocated so far]
    PartialCleanup --> SetNull[Set pointers to NULL]
    SetNull --> ReturnError[Return error]
    
    style Alloc fill:#F0E68C
    style ReadyNext fill:#90EE90
    style ExitClean fill:#90EE90
    style ReturnError fill:#FFB6C1
```

---

## 12. Data Structures

```mermaid
flowchart TD
    Structures[Data Structures] --> EnvStruct[t_env]
    Structures --> TokenStruct[t_token]
    Structures --> ASTStruct[t_ast_node]
    Structures --> RedirStruct[t_redir]
    
    EnvStruct --> EnvFields[char *name<br/>char *value<br/>t_env *next]
    
    TokenStruct --> TokenFields[t_token_type type<br/>char *value<br/>t_token *next]
    TokenFields --> TokenTypes[TOKEN_WORD<br/>TOKEN_PIPE<br/>TOKEN_REDIR_IN<br/>TOKEN_REDIR_OUT<br/>TOKEN_REDIR_APPEND<br/>TOKEN_REDIR_HEREDOC<br/>TOKEN_AND<br/>TOKEN_OR<br/>TOKEN_EOF]
    
    ASTStruct --> ASTFields[t_node_type type<br/>char **args<br/>t_ast_node *left<br/>t_ast_node *right<br/>t_redir *redirections]
    ASTFields --> NodeTypes[NODE_COMMAND<br/>NODE_PIPE<br/>NODE_AND<br/>NODE_OR]
    
    RedirStruct --> RedirFields[t_token_type type<br/>char *file<br/>t_redir *next]
    
    style Structures fill:#E6E6FA
    style EnvStruct fill:#E6E6FA
    style TokenStruct fill:#E6E6FA
    style ASTStruct fill:#E6E6FA
    style RedirStruct fill:#E6E6FA
```

---

## 13. Complete Execution Flow (End-to-End)

```mermaid
flowchart TD
    Start([🚀 Start Minishell]) --> Init[📋 Initialize Shell]
    Init --> ParseEnv[Parse Environment]
    ParseEnv --> SetupSig[Setup Signals]
    SetupSig --> MainLoop{🔄 Main REPL Loop}
    
    MainLoop --> Prompt[Display: minishell$]
    Prompt --> Read[📥 readline input]
    Read --> CheckEOF{EOF?<br/>Ctrl+D}
    CheckEOF -->|Yes| Exit[💾 Cleanup & Exit]
    CheckEOF -->|No| CheckEmpty{Empty?}
    CheckEmpty -->|Yes| MainLoop
    CheckEmpty -->|No| AddHist[Add to history]
    
    AddHist --> Lexer[🔍 LEXER: Tokenize]
    Lexer --> LexSuccess{Success?}
    LexSuccess -->|Syntax Error| PrintErr1[Print syntax error]
    PrintErr1 --> FreeTokens1[Free tokens]
    FreeTokens1 --> MainLoop
    
    LexSuccess -->|Yes| Parser[🌳 PARSER: Build AST]
    Parser --> ParseSuccess{Valid?}
    ParseSuccess -->|Syntax Error| PrintErr2[Print syntax error]
    PrintErr2 --> FreeAll1[Free tokens & AST]
    FreeAll1 --> MainLoop
    
    ParseSuccess -->|Yes| Expander[💫 EXPANDER: Expand Variables]
    Expander --> ExpandVars[Expand $VAR, $?]
    ExpandVars --> RemoveQuotes[Remove quotes]
    RemoveQuotes --> Executor[⚡ EXECUTOR: Execute]
    
    Executor --> CheckNode{Node Type?}
    
    %% Command Node
    CheckNode -->|COMMAND| CheckBuiltin{Built-in?}
    CheckBuiltin -->|Yes| ExecBuiltin[Execute Built-in]
    ExecBuiltin --> BuiltinType{Which?}
    BuiltinType -->|cd/export/unset/exit| ExecParent[Execute in Parent]
    BuiltinType -->|echo/pwd/env| ExecEither[Execute<br/>Parent or Child]
    ExecParent --> UpdateEnv[Update Environment]
    UpdateEnv --> GetStatus1[Get Exit Status]
    ExecEither --> GetStatus1
    
    CheckBuiltin -->|No| ExecExternal[Execute External Command]
    ExecExternal --> SearchPATH[Search in PATH]
    SearchPATH --> CmdFound{Found?}
    CmdFound -->|No| NotFound[Command not found]
    NotFound --> Status127[Exit status = 127]
    Status127 --> GetStatus1
    CmdFound -->|Yes| ForkExec[Fork & Exec]
    ForkExec --> ForkOK{Fork OK?}
    ForkOK -->|No| ForkErr[Fork Error]
    ForkErr --> Status1[Exit status = 1]
    Status1 --> GetStatus1
    ForkOK -->|Yes| ChildExec[Child: execve]
    ChildExec --> ParentWait[Parent: waitpid]
    ParentWait --> GetStatus1[Collect Exit Status]
    
    %% Pipe Node
    CheckNode -->|PIPE| CountCmds[Count Commands]
    CountCmds --> CreatePipes[Create Pipes<br/>n-1 pipes]
    CreatePipes --> ForkAll[Fork All Commands]
    ForkAll --> SetupFDs[Setup FDs<br/>dup2 pipe ends]
    SetupFDs --> ExecAllCmds[Execute Each Command]
    ExecAllCmds --> ClosePipes[Close All Pipe FDs]
    ClosePipes --> WaitAll[Wait for All Children]
    WaitAll --> GetLastStatus[Get Last Cmd Status]
    GetLastStatus --> GetStatus1
    
    %% Logical Operators
    CheckNode -->|AND &&| ExecLeft1[Execute Left Command]
    ExecLeft1 --> CheckLeft{Exit = 0?}
    CheckLeft -->|Yes| ExecRight1[Execute Right Command]
    CheckLeft -->|No| SkipRight1[Skip Right]
    ExecRight1 --> GetStatus1
    SkipRight1 --> GetStatus1
    
    CheckNode -->|OR or| ExecLeft2[Execute Left Command]
    ExecLeft2 --> CheckLeft2{Exit != 0?}
    CheckLeft2 -->|Yes| ExecRight2[Execute Right Command]
    CheckLeft2 -->|No| SkipRight2[Skip Right]
    ExecRight2 --> GetStatus1
    SkipRight2 --> GetStatus1
    
    %% After Execution
    GetStatus1 --> HandleRedir{Has Redirections?}
    HandleRedir -->|Yes| ApplyRedir[Apply Redirections]
    ApplyRedir --> RedirType{Type?}
    RedirType -->|Input <| OpenIn[Open file for read]
    RedirType -->|Output >| OpenOut[Open file for write]
    RedirType -->|Append >>| OpenApp[Open file for append]
    RedirType -->|Heredoc <<| ReadHeredoc[Read until delimiter]
    OpenIn --> Dup2In[dup2 to STDIN]
    OpenOut --> Dup2Out[dup2 to STDOUT]
    OpenApp --> Dup2App[dup2 to STDOUT]
    ReadHeredoc --> Dup2Here[dup2 to STDIN]
    Dup2In --> CheckRedirErr{Error?}
    Dup2Out --> CheckRedirErr
    Dup2App --> CheckRedirErr
    Dup2Here --> CheckRedirErr
    CheckRedirErr -->|Yes| RedirFail[Redirection failed]
    RedirFail --> Status1_2[Exit status = 1]
    Status1_2 --> UpdateStatus
    CheckRedirErr -->|No| RedirOK[Redirections OK]
    RedirOK --> UpdateStatus
    
    HandleRedir -->|No| UpdateStatus[Update last_exit_status]
    UpdateStatus --> Cleanup[🧹 Cleanup]
    Cleanup --> FreeLine[Free input line]
    FreeLine --> FreeTokens2[Free token list]
    FreeTokens2 --> FreeAST[Free AST tree]
    FreeAST --> RestoreFDs[Restore stdin/stdout]
    RestoreFDs --> CheckSig{Signal?}
    
    CheckSig -->|SIGINT| HandleSIGINT[Handle Ctrl+C]
    HandleSIGINT --> NewPrompt1[Display new prompt]
    NewPrompt1 --> MainLoop
    
    CheckSig -->|None| MainLoop
    
    Exit --> FreeEnv[Free environment list]
    FreeEnv --> ClearHistory[Clear readline history]
    ClearHistory --> CloseFDs[Close all FDs]
    CloseFDs --> Done([✅ Exit Shell])
    
    style Start fill:#90EE90
    style MainLoop fill:#87CEEB
    style Lexer fill:#FFD700
    style Parser fill:#FFA500
    style Expander fill:#DDA0DD
    style Executor fill:#98FB98
    style GetStatus1 fill:#98FB98
    style UpdateStatus fill:#98FB98
    style Cleanup fill:#87CEEB
    style Done fill:#90EE90
    style PrintErr1 fill:#FFB6C1
    style PrintErr2 fill:#FFB6C1
    style NotFound fill:#FFB6C1
    style ForkErr fill:#FFB6C1
    style RedirFail fill:#FFB6C1
```

---

## 14. High-Level System Flow (Simplified)

```mermaid
flowchart LR
    User[User Input] --> Readline[readline]
    Readline --> Lexer[LEXER]
    Lexer --> Parser[PARSER]
    Parser --> Expander[EXPANDER]
    Expander --> Executor[EXECUTOR]
    Executor --> Output[Output/Status]
    Output --> Prompt[New Prompt]
    Prompt --> User
    
    style User fill:#87CEEB
    style Lexer fill:#FFD700
    style Parser fill:#FFA500
    style Expander fill:#DDA0DD
    style Executor fill:#98FB98
    style Output fill:#90EE90
```

---

## 15. Detailed Executor Decision Tree

```mermaid
flowchart TD
    Exec[⚡ EXECUTOR Entry] --> CheckAST{AST Node Type?}
    
    %% COMMAND NODE FLOW
    CheckAST -->|NODE_COMMAND| SaveFDs[💾 Save stdin/stdout]
    SaveFDs --> ApplyRedirFirst{Has Redirections?}
    ApplyRedirFirst -->|Yes| ProcessRedir[Process Redirections]
    ProcessRedir --> RedirLoop{For each redir}
    RedirLoop --> RedirType{Type?}
    
    RedirType -->|<| HandleInput[Handle Input <]
    HandleInput --> OpenFile1[open file O_RDONLY]
    OpenFile1 --> CheckOpen1{Success?}
    CheckOpen1 -->|No| FileErr1[Error: No such file]
    CheckOpen1 -->|Yes| Dup2_1[dup2 fd → STDIN]
    Dup2_1 --> Close1[close fd]
    Close1 --> RedirLoop
    
    RedirType -->|>| HandleOutput[Handle Output >]
    HandleOutput --> OpenFile2[open file O_WRONLY  O_CREAT O_TRUNC]
    OpenFile2 --> CheckOpen2{Success?}
    CheckOpen2 -->|No| FileErr2[Error: Permission denied]
    CheckOpen2 -->|Yes| Dup2_2[dup2 fd → STDOUT]
    Dup2_2 --> Close2[close fd]
    Close2 --> RedirLoop
    
    RedirType -->|>>| HandleAppend[Handle Append >>]
    HandleAppend --> OpenFile3[open file O_WRONLY O_CREAT O_APPEND]
    OpenFile3 --> CheckOpen3{Success?}
    CheckOpen3 -->|No| FileErr3[Error: Permission denied]
    CheckOpen3 -->|Yes| Dup2_3[dup2 fd → STDOUT]
    Dup2_3 --> Close3[close fd]
    Close3 --> RedirLoop
    
    RedirType -->|<<| HandleHeredoc[Handle Heredoc <<]
    HandleHeredoc --> PipeCreate[pipe fds]
    PipeCreate --> ReadLines[Read lines until delimiter]
    ReadLines --> ExpandHere{Expand vars?}
    ExpandHere -->|Yes| ExpandLine[Expand $VAR]
    ExpandHere -->|No| KeepRaw[Keep raw]
    ExpandLine --> WriteToP[write to pipe]
    KeepRaw --> WriteToP
    WriteToP --> MoreLines{More lines?}
    MoreLines -->|Yes| ReadLines
    MoreLines -->|No| CloseWrite[close write end]
    CloseWrite --> Dup2_4[dup2 read → STDIN]
    Dup2_4 --> CloseRead[close read end]
    CloseRead --> RedirLoop
    
    RedirLoop -->|More| RedirType
    RedirLoop -->|Done| ExecCmd[Execute Command]
    ApplyRedirFirst -->|No| ExecCmd
    
    FileErr1 --> RestoreFDs1[Restore FDs]
    FileErr2 --> RestoreFDs1
    FileErr3 --> RestoreFDs1
    RestoreFDs1 --> Return1[Return 1]
    
    ExecCmd --> IsBuiltin{Is Built-in?}
    
    %% BUILT-IN EXECUTION
    IsBuiltin -->|Yes| WhichBuiltin{Which Built-in?}
    WhichBuiltin -->|echo| CallEcho[builtin_echo]
    WhichBuiltin -->|cd| CallCD[builtin_cd]
    WhichBuiltin -->|pwd| CallPWD[builtin_pwd]
    WhichBuiltin -->|export| CallExport[builtin_export]
    WhichBuiltin -->|unset| CallUnset[builtin_unset]
    WhichBuiltin -->|env| CallEnv[builtin_env]
    WhichBuiltin -->|exit| CallExit[builtin_exit]
    
    CallEcho --> BuiltinReturn[Get return status]
    CallCD --> UpdatePWD[Update PWD/OLDPWD]
    UpdatePWD --> BuiltinReturn
    CallPWD --> BuiltinReturn
    CallExport --> ModifyEnv1[Modify environment]
    ModifyEnv1 --> BuiltinReturn
    CallUnset --> ModifyEnv2[Modify environment]
    ModifyEnv2 --> BuiltinReturn
    CallEnv --> BuiltinReturn
    CallExit --> ExitShell1[Exit shell]
    
    BuiltinReturn --> RestoreFDs2[Restore stdin/stdout]
    RestoreFDs2 --> ReturnStatus[Return status]
    
    %% EXTERNAL COMMAND
    IsBuiltin -->|No| FindCmd[Find Command]
    FindCmd --> HasSlash{Has '/' ?}
    HasSlash -->|Yes| UseAbsPath[Use as path]
    HasSlash -->|No| GetPATH[Get PATH env]
    GetPATH --> SplitPATH[Split by ':']
    SplitPATH --> TryDirs{Try each dir}
    TryDirs --> BuildPath[Build dir/cmd]
    BuildPath --> TestAccess[access X_OK]
    TestAccess --> Accessible{Accessible?}
    Accessible -->|No| NextDir[Next directory]
    NextDir --> TryDirs
    Accessible -->|Yes| FoundCmd[Command found]
    TryDirs -->|No more| CmdNotFound[Command not found]
    CmdNotFound --> Print127[Print error]
    Print127 --> Return127[Return 127]
    
    UseAbsPath --> TestAbs[access X_OK]
    TestAbs --> AbsOK{OK?}
    AbsOK -->|No| PermDenied[Permission denied]
    PermDenied --> Return126[Return 126]
    AbsOK -->|Yes| FoundCmd
    
    FoundCmd --> ForkProcess[fork]
    ForkProcess --> ForkSuccess{Success?}
    ForkSuccess -->|No| ForkError[Fork failed]
    ForkError --> Return1_2[Return 1]
    
    ForkSuccess -->|Yes| InChild{Child process?}
    InChild -->|Yes| ConvertEnv[env_list → array]
    ConvertEnv --> CallExecve[execve path args env]
    CallExecve --> ExecFailed[execve failed]
    ExecFailed --> ExitChild[exit 126]
    
    InChild -->|No| WaitChild[waitpid child]
    WaitChild --> Terminated{How terminated?}
    Terminated -->|WIFEXITED| GetExitStat[WEXITSTATUS]
    Terminated -->|WIFSIGNALED| GetSignal[128 + WTERMSIG]
    GetExitStat --> RestoreFDs3[Restore FDs]
    GetSignal --> RestoreFDs3
    RestoreFDs3 --> ReturnStatus
    
    %% PIPE NODE FLOW
    CheckAST -->|NODE_PIPE| CountPipeCmds[Count commands in pipeline]
    CountPipeCmds --> CalcPipes[Need n-1 pipes]
    CalcPipes --> AllocPipes[Create all pipe pairs]
    AllocPipes --> PipeErr{Pipe error?}
    PipeErr -->|Yes| ReturnPipeErr[Return 1]
    PipeErr -->|No| ForkLoop{For each command}
    
    ForkLoop --> ForkPipeCmd[fork]
    ForkPipeCmd --> ForkOK2{Success?}
    ForkOK2 -->|No| KillAll[Kill all children]
    KillAll --> ReturnErr2[Return 1]
    
    ForkOK2 -->|Yes| InPipeChild{In child?}
    InPipeChild -->|Yes| WhichPos{Position?}
    WhichPos -->|First| Setup1[dup2 pipe.0.1 → STDOUT]
    WhichPos -->|Middle| Setup2[dup2 pipe.i.0 → STDIN<br/>dup2 pipe.i+1.1 → STDOUT]
    WhichPos -->|Last| Setup3[dup2 pipe.n-1.0 → STDIN]
    
    Setup1 --> CloseAllPipes1[Close all pipe FDs]
    Setup2 --> CloseAllPipes1
    Setup3 --> CloseAllPipes1
    CloseAllPipes1 --> ExecPipeCmd[Execute command<br/>recursive call]
    ExecPipeCmd --> ExitPipeChild[exit with status]
    
    InPipeChild -->|No| StorePID[Store child PID]
    StorePID --> MorePipeCmds{More commands?}
    MorePipeCmds -->|Yes| ForkLoop
    MorePipeCmds -->|No| CloseAllPipes2[Parent: Close all pipes]
    CloseAllPipes2 --> WaitLoop{Wait for all}
    WaitLoop --> WaitPID[waitpid -1]
    WaitPID --> WaitLoop
    WaitLoop -->|All done| GetLastPipe[Get last command status]
    GetLastPipe --> ReturnPipeStat[Return status]
    
    %% LOGICAL OPERATORS
    CheckAST -->|NODE_AND| ExecLeftAnd[Execute left subtree]
    ExecLeftAnd --> CheckAndStatus{Status == 0?}
    CheckAndStatus -->|Yes| ExecRightAnd[Execute right subtree]
    CheckAndStatus -->|No| ReturnLeftAnd[Return left status]
    ExecRightAnd --> ReturnRightAnd[Return right status]
    
    CheckAST -->|NODE_OR| ExecLeftOr[Execute left subtree]
    ExecLeftOr --> CheckOrStatus{Status != 0?}
    CheckOrStatus -->|Yes| ExecRightOr[Execute right subtree]
    CheckOrStatus -->|No| ReturnLeftOr[Return left status]
    ExecRightOr --> ReturnRightOr[Return right status]
    
    %% RETURN POINTS
    Return1 --> End[Return to Main Loop]
    Return127 --> End
    Return126 --> End
    Return1_2 --> End
    ReturnStatus --> End
    ReturnPipeErr --> End
    ReturnErr2 --> End
    ReturnPipeStat --> End
    ReturnLeftAnd --> End
    ReturnRightAnd --> End
    ReturnLeftOr --> End
    ReturnRightOr --> End
    ExitShell1 --> ExitDone([Shell Exits])
    
    style Exec fill:#98FB98
    style ExecCmd fill:#98FB98
    style BuiltinReturn fill:#90EE90
    style ReturnStatus fill:#90EE90
    style End fill:#87CEEB
    style FileErr1 fill:#FFB6C1
    style FileErr2 fill:#FFB6C1
    style FileErr3 fill:#FFB6C1
    style CmdNotFound fill:#FFB6C1
    style PermDenied fill:#FFB6C1
    style ForkError fill:#FFB6C1
    style ExitDone fill:#90EE90
```

---

## 16. Built-in Decision Flow

```mermaid
flowchart TD
    Cmd[Command to Execute] --> IsBuiltin{Is built-in?}
    IsBuiltin -->|No| External[Execute External]
    IsBuiltin -->|Yes| HasPipe{Has pipe or<br/>redirection?}
    HasPipe -->|Yes| Fork[Fork child process]
    HasPipe -->|No| NeedEnv{Needs parent env?}
    
    NeedEnv -->|Yes| ExecParent[Execute in parent<br/>cd, export, unset, exit]
    NeedEnv -->|No| CanFork[Can fork<br/>echo, pwd, env]
    CanFork --> ExecParent
    
    Fork --> ExecChild[Execute in child]
    ExecChild --> ExitChild[exit with status]
    ExitChild --> ParentWait[Parent waits]
    ParentWait --> Done1[Done]
    
    ExecParent --> ModifyEnv{Modifies env?}
    ModifyEnv -->|Yes| UpdateParent[Update parent env]
    ModifyEnv -->|No| JustReturn[Return status]
    UpdateParent --> Done2[Done]
    JustReturn --> Done2
    
    External --> ForkExt[Fork and exec]
    ForkExt --> Done3[Done]
    
    style Cmd fill:#98FB98
    style Done1 fill:#90EE90
    style Done2 fill:#90EE90
    style Done3 fill:#90EE90
```

---

## 17. Error Handling Flow

```mermaid
flowchart TD
    Error[Error Detected] --> CheckType{Error Type?}
    
    CheckType -->|Syntax Error| PrintSyntax[Print syntax error message]
    PrintSyntax --> FreePartial1[Free allocated memory]
    FreePartial1 --> SetStatus1[Set exit status = 2]
    SetStatus1 --> Return1[Return to prompt]
    
    CheckType -->|Command Not Found| PrintNotFound[Print: command not found]
    PrintNotFound --> SetStatus2[Set exit status = 127]
    SetStatus2 --> Return2[Return to prompt]
    
    CheckType -->|Permission Denied| PrintPerm[Print: permission denied]
    PrintPerm --> SetStatus3[Set exit status = 126]
    SetStatus3 --> Return3[Return to prompt]
    
    CheckType -->|Fork Error| PrintFork[Print: fork error]
    PrintFork --> FreePartial2[Free allocated memory]
    FreePartial2 --> SetStatus4[Set exit status = 1]
    SetStatus4 --> Return4[Return to prompt]
    
    CheckType -->|File Error| PrintFile[Print: file error]
    PrintFile --> ChildExit[Child exits with 1]
    ChildExit --> ParentCollects[Parent collects status]
    ParentCollects --> Return5[Return to prompt]
    
    CheckType -->|Memory Error| PrintMem[Print: malloc error]
    PrintMem --> Cleanup[Cleanup all resources]
    Cleanup --> ExitShell[Exit shell]
    
    style Error fill:#FFB6C1
    style ExitShell fill:#FFB6C1
```

---

## 18. Command Execution Summary Table

| Command Type | Fork Required? | Modifies Parent Env? | Exit Behavior | Notes |
|-------------|----------------|---------------------|---------------|-------|
| **echo** | Optional | No | Returns 0 | Can run in parent or child |
| **cd** | Never | Yes (PWD/OLDPWD) | Returns 0/1 | Must run in parent |
| **pwd** | Optional | No | Returns 0/1 | Can run in parent or child |
| **export** | Never | Yes | Returns 0/1 | Must run in parent |
| **unset** | Never | Yes | Returns 0 | Must run in parent |
| **env** | Optional | No | Returns 0 | Can run in parent or child |
| **exit** | Never | No | Exits shell | Terminates minishell |
| **External** | Always | No | Returns child status | fork + execve required |
| **Pipeline** | Always (n times) | No | Returns last status | Each command forks |

### Key Decision Rules:

1. **Environment Modifiers** (cd, export, unset, exit) → Execute in **parent process**
2. **Read-only Built-ins** (echo, pwd, env) → Execute in **parent** (unless in pipeline)
3. **Pipelines** → Every command forks, even built-ins
4. **Redirections** → Apply before execution, restore after
5. **Logical Operators** → Short-circuit evaluation (&&, ||)

---

## 19. Memory Lifecycle

```mermaid
flowchart TD
    Alloc[Memory Allocation Points] --> Point1[readline → line]
    Alloc --> Point2[Lexer → tokens]
    Alloc --> Point3[Parser → AST]
    Alloc --> Point4[Expander → expanded strings]
    Alloc --> Point5[Executor → temporary buffers]
    
    Point1 --> Track1[Tracked Pointers]
    Point2 --> Track1
    Point3 --> Track1
    Point4 --> Track1
    Point5 --> Track1
    
    Track1 --> PerCommand[After Each Command]
    PerCommand --> Free1[free line]
    Free1 --> Free2[free tokens recursively]
    Free2 --> Free3[free AST recursively]
    Free3 --> Free4[free expanded strings]
    Free4 --> Free5[free temporary buffers]
    Free5 --> Ready[Ready for next command]
    
    Ready --> LoopAgain{Continue?}
    LoopAgain -->|Yes| Alloc
    LoopAgain -->|No| OnExit[On Shell Exit]
    
    OnExit --> FinalFree1[Free environment list]
    FinalFree1 --> FinalFree2[rl_clear_history]
    FinalFree2 --> FinalFree3[Close all FDs]
    FinalFree3 --> Clean[Clean exit]
    
    Error[Error During Execution] --> PartialFree[Free allocated so far]
    PartialFree --> NullPtrs[Set pointers to NULL]
    NullPtrs --> ReturnErr[Return error status]
    
    style Alloc fill:#F0E68C
    style PerCommand fill:#87CEEB
    style Ready fill:#90EE90
    style Clean fill:#90EE90
    style Error fill:#FFB6C1
```

---

## 20. Signal Handling States

```mermaid
stateDiagram-v2
    [*] --> Interactive: Start Shell
    
    Interactive --> Reading: Display Prompt
    Reading --> Interactive: Empty Input
    Reading --> Executing: Valid Command
    
    Executing --> Interactive: Command Done
    Executing --> Interrupted: SIGINT (Ctrl+C)
    Interrupted --> Interactive: Display New Prompt
    
    Interactive --> Exiting: EOF (Ctrl+D)
    Executing --> Exiting: exit command
    Exiting --> [*]: Clean Exit
    
    note right of Interactive
        SIGINT: Display new line + prompt
        SIGQUIT: Ignored
    end note
    
    note right of Executing
        SIGINT: Terminates child
        SIGQUIT: May core dump (child)
        Parent waits for child
    end note
    
    note right of Reading
        Blocking on readline()
        Can be interrupted by SIGINT
    end note
```

---

## Usage Instructions

### For Excalidraw Import:

1. **Copy Individual Diagrams**: Copy each Mermaid code block separately
2. **Convert to Image**: Use [Mermaid Live Editor](https://mermaid.live) to convert to SVG
3. **Import to Excalidraw**: Import SVG into Excalidraw
4. **Arrange**: Position diagrams as needed
5. **Connect**: Add custom arrows between diagrams
6. **Annotate**: Add notes and labels in Excalidraw

### Alternative Method:

1. Use Excalidraw's built-in Mermaid support (if available)
2. Paste Mermaid code directly into Excalidraw
3. Excalidraw will render the diagram

### Tips:

- Each diagram is self-contained and modular
- Diagrams use consistent color coding:
  - 🟢 Green: Success/Start/End
  - 🔴 Pink: Errors
  - 🔵 Blue: Main loop
  - 🟡 Yellow: Lexer
  - 🟠 Orange: Parser
  - 🟣 Purple: Expander
  - 🟢 Light Green: Executor
  - 🟡 Light Yellow: Signals/Memory

---

**Document Version**: 1.0  
**Last Updated**: November 5, 2025  
**Total Diagrams**: 15 modular components  
**Optimized for**: Excalidraw import
