/* eslint no-console: 0 */
const path = require('path');
const express = require('express');
const webpack = require('webpack');
const webpackMiddleware = require('webpack-dev-middleware');
const webpackHotMiddleware = require('webpack-hot-middleware');
const config = require('./webpack.config');

// create express
const app = express();

// setup plugins
config.plugins = [];

// hot reload plugin
config.plugins.push(new webpack.HotModuleReplacementPlugin());
// setup no erros plugin
config.plugins.push(new webpack.NoEmitOnErrorsPlugin());
// override entry for hot reload
config.entry = [
  'webpack-hot-middleware/client',
  config.entry,
];
// returns a Compiler instance
const compiler = webpack(config);
// stats output config
const statsConf = {
  colors: true,
  hash: false,
  timings: true,
  chunks: false,
  chunkModules: false,
  modules: false,
};
// add hot reload middleware
app.use(webpackMiddleware(compiler, {
  publicPath: config.output.publicPath,
  contentBase: 'src',
  stats: statsConf,
}));
app.use(webpackHotMiddleware(compiler));

app.use(express.static(__dirname));
app.get('*', (req, res) => res.sendFile(path.join(__dirname, 'index.html')));
app.listen(3000, (err) => {
  if (err) {
    console.log(err);
  }
  console.info('==> Listening on port 3000');
});
