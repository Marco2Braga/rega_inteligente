// Variáveis globais
let dadosCSV = [];
let configuracoes = {};



// Inicializar quando o DOM estiver pronto
document.addEventListener('DOMContentLoaded', function() {
    inicializarEventos();
    carregarDadosAutomaticamente(); // Novo: Carrega o CSV automaticamente
});

// Inicializar eventos
function inicializarEventos() {
    // Evento de filtro de linhas
    document.getElementById('filterRows').addEventListener('change', atualizarTabela);
}

// Carregar dados do CSV automaticamente via Fetch
function carregarDadosAutomaticamente() {
    const caminhoCSV = 'dados_horta.csv'; // Assumindo que o arquivo está no mesmo diretório
    
    fetch(caminhoCSV)
        .then(response => {
            if (!response.ok) {
                throw new Error(`Erro ao carregar o arquivo: ${response.statusText}`);
            }
            return response.text();
        })
        .then(csvText => {
            processarCSV(csvText);
        })
        .catch(error => {
            mostrarMensagem(`❌ Erro ao carregar dados: ${error.message}`, 'error');
        });
}

// Processar texto CSV
function processarCSV(csv) {
    try {
        dadosCSV = parseCSV(csv);
        
        if (dadosCSV.length === 0) {
            mostrarMensagem('Arquivo CSV vazio ou formato inválido.', 'error');
            return;
        }
        
        atualizarDashboard();
        mostrarMensagem(`✅ Dados carregados automaticamente! ${dadosCSV.length} registros encontrados.`, 'success');
    } catch (error) {
        mostrarMensagem(`❌ Erro ao processar dados: ${error.message}`, 'error');
    }
}

// Parser de CSV simples
function parseCSV(csv) {
    const linhas = csv.trim().split('\n');
    if (linhas.length < 2) return [];
    
    // Extrair cabeçalho
    const cabecalho = linhas[0].split(',').map(h => h.trim().toLowerCase());
    
    // Validar cabeçalho esperado
    const colunas = {
        data_hora: cabecalho.indexOf('data_hora'),
        contador_bomba: cabecalho.indexOf('contador_bomba'),
        umidade_solo: cabecalho.indexOf('umidade_solo')
    };
    
    if (colunas.data_hora === -1 || colunas.contador_bomba === -1 || colunas.umidade_solo === -1) {
        throw new Error('Arquivo CSV deve conter as colunas: data_hora, umidade_solo, contador_bomba');
    }
    
    // Processar dados
    const dados = [];
    for (let i = 1; i < linhas.length; i++) {
        const linha = linhas[i].split(',').map(v => v.trim());
        
        if (linha.length >= 3 && linha[colunas.data_hora]) {
            // Converter umidade de 0-4095 para 0-100% (0=100% úmido, 4095=0% úmido)
            // Umidade % = 100 - (valor_raw * 100 / 4095)
            const umidadeRaw = parseFloat(linha[colunas.umidade_solo]) || 0;
            const umidadePercentual = 100 - (umidadeRaw * 100 / 4095);
            
            dados.push({
                data_hora: linha[colunas.data_hora],
                umidade_solo_raw: umidadeRaw,
                umidade_solo_percent: umidadePercentual,
                contador_bomba: parseInt(linha[colunas.contador_bomba]) || 0
            });
        }
    }
    
    // Ordenar por data (mais recentes primeiro)
    dados.sort((a, b) => new Date(b.data_hora) - new Date(a.data_hora));
    
    return dados;
}

// Atualizar dashboard com dados
function atualizarDashboard() {
    if (dadosCSV.length === 0) return;
    
    // Atualizar cards da página inicial
    atualizarCardBomba();
    atualizarCardUmidade();
    atualizarCardInfo();
    
    // Atualizar tabela
    atualizarTabela();
    
    // Atualizar gráficos e estatísticas
    atualizarGraficos();
}

// Atualizar card da bomba
function atualizarCardBomba() {
    const ultimoRegistro = dadosCSV[0];
    const contadorBomba = ultimoRegistro.contador_bomba;
    
    // O status da bomba agora é baseado no contador. Se o contador for maior que o registro anterior, a bomba ligou.
    // Como estamos apenas exibindo o último estado, vamos focar no contador total.
    
    // O status atual é o valor do contador no último registro.
    document.getElementById('bomba-status').textContent = contadorBomba > 0 ? 'Ativações Registradas' : 'Nenhuma Ativação';
    document.getElementById('bomba-status').className = `status-badge ${contadorBomba > 0 ? 'ligada' : 'desligada'}`;
    document.getElementById('bomba-tempo').textContent = formatarData(ultimoRegistro.data_hora);
    
    // O total de ativações é o valor do contador no último registro.
    document.getElementById('bomba-total').textContent = contadorBomba;
}

// Atualizar card de umidade
function atualizarCardUmidade() {
    const ultimoRegistro = dadosCSV[0];
    const umidade = ultimoRegistro.umidade_solo_percent;
    
    document.getElementById('umidade-atual').textContent = umidade.toFixed(1);
    document.getElementById('umidade-tempo').textContent = formatarData(ultimoRegistro.data_hora);
    
    // Calcular média
    const media = dadosCSV.reduce((acc, d) => acc + d.umidade_solo_percent, 0) / dadosCSV.length;
    document.getElementById('umidade-media').textContent = media.toFixed(1);
}

// Atualizar card de informações
function atualizarCardInfo() {
    document.getElementById('total-registros').textContent = dadosCSV.length;
    
    if (dadosCSV.length > 0) {
        const primeiroRegistro = dadosCSV[dadosCSV.length - 1];
        const ultimoRegistro = dadosCSV[0];
        const periodo = `${formatarData(primeiroRegistro.data_hora)} até ${formatarData(ultimoRegistro.data_hora)}`;
        document.getElementById('periodo-dados').textContent = periodo;
        document.getElementById('status-arquivo').textContent = '✅ Arquivo carregado';
    }
}


// Atualizar tabela de monitoramento
function atualizarTabela() {
    const filterRows = parseInt(document.getElementById('filterRows').value);
    const dadosExibir = dadosCSV.slice(0, filterRows);
    const tableBody = document.getElementById('tableBody');
    
    if (dadosExibir.length === 0) {
        tableBody.innerHTML = '<tr><td colspan="3" style="text-align: center; color: #999;">Nenhum dado disponível</td></tr>';
        return;
    }
    
    tableBody.innerHTML = dadosExibir.map(registro => `
        <tr>
            <td>${formatarData(registro.data_hora)}</td>
            <td>${registro.umidade_solo_raw} ( ${registro.umidade_solo_percent.toFixed(1)}% )</td>
            <td>${registro.contador_bomba}</td>
        </tr>
    `).join('');
}

// Atualizar gráficos e estatísticas
function atualizarGraficos() {
    if (dadosCSV.length === 0) return;
    
    // Calcular estatísticas
    const umidades = dadosCSV.map(d => d.umidade_solo_percent);
    const maxUmidade = Math.max(...umidades);
    const minUmidade = Math.min(...umidades);
    const avgUmidade = (umidades.reduce((a, b) => a + b, 0) / umidades.length).toFixed(1);
    
    // O total de ativações é o valor do contador no último registro.
    const totalAtivacoes = dadosCSV.length > 0 ? dadosCSV[0].contador_bomba : 0;
    
    document.getElementById('stat-max').textContent = maxUmidade.toFixed(1) + '%';
    document.getElementById('stat-min').textContent = minUmidade.toFixed(1) + '%';
    document.getElementById('stat-avg').textContent = avgUmidade + '%';
    document.getElementById('stat-bomba').textContent = totalAtivacoes;
    
    // Criar gráfico de umidade
    criarGraficoUmidade(dadosCSV);
}

// Criar gráfico de umidade
function criarGraficoUmidade(dados) {
    const ctx = document.getElementById('umidadeChart');
    
    if (!ctx) return;
    
    // Limitar a 24 últimos registros para melhor visualização
    const dadosGrafico = dados.slice(0, 24).reverse();
    
    const labels = dadosGrafico.map(d => formatarHora(d.data_hora));
    const valores = dadosGrafico.map(d => d.umidade_solo_percent);
    
    // Destruir gráfico anterior se existir
    if (window.graficoUmidade) {
        window.graficoUmidade.destroy();
    }
    
    window.graficoUmidade = new Chart(ctx, {
        type: 'line',
        data: {
            labels: labels,
            datasets: [{
                label: 'Umidade do Solo (%)',
                data: valores,
                borderColor: '#28a745',
                backgroundColor: 'rgba(40, 167, 69, 0.1)',
                borderWidth: 2,
                fill: true,
                tension: 0.4,
                pointRadius: 4,
                pointBackgroundColor: '#28a745',
                pointBorderColor: '#fff',
                pointBorderWidth: 2
            }]
        },
        options: {
            responsive: true,
            maintainAspectRatio: true,
            plugins: {
                legend: {
                    display: true,
                    position: 'top'
                }
            },
            scales: {
                y: {
                    beginAtZero: true,
                    max: 100,
                    title: {
                        display: true,
                        text: 'Umidade (%)'
                    }
                }
            }
        }
    });
}

// Mostrar/ocultar páginas
function showPage(pageId) {
    const allSections = document.querySelectorAll('#content-area > .content-section');
    allSections.forEach(section => section.classList.add('hidden'));
    const targetSection = document.getElementById(pageId);
    if (targetSection) targetSection.classList.remove('hidden');
}

// Mostrar mensagem de status
function mostrarMensagem(mensagem, tipo = 'info') {
    const statusEl = document.getElementById('uploadStatus');
    statusEl.textContent = mensagem;
    statusEl.className = `status-message ${tipo}`;
    
    // Limpar mensagem após 5 segundos
    setTimeout(() => {
        statusEl.textContent = '';
        statusEl.className = 'status-message';
    }, 5000);
}

// Formatar data completa
function formatarData(dataString) {
    try {
        const data = new Date(dataString);
        const dia = String(data.getDate()).padStart(2, '0');
        const mes = String(data.getMonth() + 1).padStart(2, '0');
        const ano = data.getFullYear();
        const horas = String(data.getHours()).padStart(2, '0');
        const minutos = String(data.getMinutes()).padStart(2, '0');
        return `${dia}/${mes}/${ano} ${horas}:${minutos}`;
    } catch (e) {
        return dataString;
    }
}

// Formatar apenas hora
function formatarHora(dataString) {
    try {
        const data = new Date(dataString);
        const horas = String(data.getHours()).padStart(2, '0');
        const minutos = String(data.getMinutes()).padStart(2, '0');
        return `${horas}:${minutos}`;
    } catch (e) {
        return dataString;
    }
}



// Inicializar página
window.onload = function() {
    showPage('page-inicio');
};
