# 🌱 Projeto de Automação de Cultivo Indoor

![Status](https://img.shields.io/badge/status-em%20desenvolvimento-yellow)
![Versão](https://img.shields.io/badge/versão-0.1.0-blue)
![Licença](https://img.shields.io/badge/licença-privado-red)

### Índice
- [Sobre o Projeto 🪴](#sobre-o-projeto-)
- [Como Contribuir 🤝](#como-contribuir-)
- [Roadmap & To-Do List 📝](#roadmap--to-do-list-)

---

## Sobre o Projeto 🪴

O objeti é construir um protótipo de sistema de automação para cultivo de plantas em ambiente interno (indoor), projetado para simplificar e otimizar o cuidado com as plantas. Utilizando uma combinação de hardware (microcontroladores e sensores) e software, o projeto visa monitorar e controlar as condições vitais do cultivo, tornando-o acessível tanto para iniciantes quanto para entusiastas experientes.

### Principais Funcionalidades
* 💧 **Rega Automatizada:** Acionamento inteligente de uma bomba d'água com base nos níveis de umidade do solo.
* 🌡️ **Monitoramento de Ambiente:** Sensores para medir a umidade do ar, temperatura e intensidade da luz.
* 📊 **Dashboard Online:** Uma interface web para visualização dos dados históricos e atuais dos sensores em gráficos.
* 🎛️ **Controle Remoto:** Possibilidade de acionar a rega ou outras funções manualmente através da interface.
* 🔔 **Alertas e Notificações:** (Futuro) Envio de alertas quando os parâmetros saem do ideal.

---

## Como Contribuir 🤝

Este é um repositório **privado**. Para contribuir, você precisa ser um membro convidado. Nosso fluxo de trabalho é baseado no **Feature Branch Workflow**, o que significa que não usamos *forks*. Todo o trabalho é feito em *branches* dentro deste mesmo repositório.

Siga o passo a passo abaixo para adicionar suas contribuições:

### Passo 1: Clonar o Repositório
Se você ainda não tem o projeto na sua máquina, clone-o usando o comando abaixo.

```bash
git clone <URL_DO_REPOSITORIO_AQUI>
cd <nome-do-repositorio>
```


### Passo 2: Criar uma nova branch
Nunca trabalhe diretamente nas branches main. Para cada nova funcionalidade ou correção, crie sua própria branch a partir da branch principal (main).

Dica: Use um nome descritivo, como feature/tela-de-login ou fix/bug-na-api.

```bash
# Certifique-se de estar na branch principal e com ela atualizada
git checkout main
git pull origin main

# Crie sua nova branch e mude para ela
git checkout -b feature/nome-da-sua-feature
```

### Passo 3: Desenvolver e Commitar
Faça suas alterações no código. Adicione e "commite" seu trabalho em pequenos passos lógicos. Escreva mensagens de commit claras e objetivas!

```bash
# Adiciona todos os arquivos modificados para a área de "stage"
git add .

# Grava as alterações com uma mensagem descritiva
git commit -m "feat: adiciona componente de gráfico de temperatura"
```

#Passo 4: Enviar a Branch para o Repositório Remoto
Envie sua branch local para o repositório remoto no GitHub/GitLab.

```bash
git push origin feature/nome-da-sua-feature
```
### Passo 5: Abrir o Pull Request (PR)
+ Vá para a página do repositório no seu navegador.

+ Assim que você enviar sua branch, o GitHub (ou similar) mostrará uma barra de aviso amarela/verde no topo da página com o nome da sua branch e um botão "Compare & pull request". Clique nele!

+ Na tela de criação do Pull Request:

+ Escreva um título claro para o seu PR.

+ Na descrição, explique o que você fez e por quê. Se o PR resolve uma tarefa do To-Do, mencione-a.

+ Na lateral direita, adicione os outros membros do grupo como "Reviewers" (revisores) para que eles possam analisar seu código.

+ Clique em "Create pull request".

### Passo 6: Revisão e Merge
Aguarde a revisão do seu código pelos outros membros. Participe das discussões e faça os ajustes necessários. Após a aprovação, um dos responsáveis pelo repositório fará o merge do seu Pull Request.

### Passo 7: fetch/pull

Lembre-se sempre de conferir se seu código está com a versão mais atual disponível no repositório!

```bash
git fetch
git pull
```

---

## Roadmap & To-Do List 📝

Esta lista define as próximas tarefas e o futuro do projeto. Sinta-se à vontade para pegar uma tarefa, criar uma branch e começar a trabalhar!

_Marque as caixas com um `x` (`- [x]`) quando a tarefa for concluída._

### 🔩 Hardware & Sensores
- [ ] Definir o microcontrolador principal (ESP32, Raspberry Pi Pico W, etc.).
- [ ] Selecionar e testar o sensor de umidade do solo.
- [ ] Selecionar e testar o sensor de temperatura e umidade do ar (DHT22, BME280).
- [ ] Montar o circuito de acionamento do relé para a bomba d'água.
- [ ] Projetar e imprimir em 3D um case para o dispositivo.

### 💻 Backend & Lógica de Controle
- [ ] Criar a estrutura inicial do projeto (API REST).
- [ ] Desenvolver endpoint para receber e salvar as leituras dos sensores.
- [ ] Implementar a lógica de decisão para a rega automática.
- [ ] Configurar um banco de dados (SQLite, PostgreSQL) para armazenar o histórico de medições.
- [ ] Criar endpoint para acionamento manual da rega.
- [ ] Construir biblioteca com melhores condições para algumas plantas, permitindo melhor cuidado.

### 🌐 Frontend & Dashboard
- [ ] Desenvolver o protótipo da interface (layout, wireframe).
- [ ] Criar o componente de gráfico para exibir dados históricos (Ex: temperatura ao longo do dia).
- [ ] Implementar a visualização dos dados dos sensores em tempo real.
- [ ] Criar o botão e a lógica para o controle manual da bomba.
- [ ] Tornar a interface responsiva para acesso via celular.

### ⚙️ Geral & DevOps
- [ ] Escrever a documentação inicial da API.
- [ ] Configurar um ambiente de desenvolvimento padronizado (Docker, se necessário).