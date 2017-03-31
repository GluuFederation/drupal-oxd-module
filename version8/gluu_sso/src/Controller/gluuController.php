<?php

namespace Drupal\gluu_sso\Controller;

use Drupal\Core\Controller\ControllerInterface;
use Drupal\Core\DependencyInjection\ContainerInjectionInterface;
use Symfony\Component\DependencyInjection\ContainerInterface;

class gluuController implements ContainerInjectionInterface {

  public static function create(ContainerInterface $container) {
    return new static($container->get('module_handler'));
  }

  public function loginPage() {
    $build = array(
      '#type' => 'markup',
      '#markup' => t('Hello World!'),
    );
    return $build;
  }
}
